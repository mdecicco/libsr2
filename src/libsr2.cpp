#include <libsr2/utilities/GameArchive.h>
#include <libsr2/utilities/Data.h>
#include <libsr2/utilities/datArgParser.h>
#include <libsr2/utilities/timer.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/utilities/msgMsgSource.h>
#include <libsr2/utilities/txtFontTex.h>
#include <libsr2/states/gameFSM.h>
#include <libsr2/states/GameState.h>
#include <libsr2/managers/srAudMgr.h>
#include <libsr2/managers/Aud3DObjectManager.h>
#include <libsr2/managers/datTimeManager.h>
#include <libsr2/gfx/gfx.h>
#include <libsr2/gfx/gfxTextureMovie.h>
#include <libsr2/ui/GameLoadingScreen.h>
#include <libsr2/io/ioKeyboard.h>
#include <libsr2/io/ioPad.h>
#include <libsr2/libsr2.h>
#include <libsr2/globals.h>
#include <libsr2/gfx/ui.h>

#include <utils/Window.h>
#include <utils/Singleton.hpp>
#include <utils/Exception.h>

#include <render/utils/SimpleDebugDraw.h>
#include <render/utils/ImGui.h>
#include <render/vulkan/CommandBuffer.h>
#include <render/vulkan/Instance.h>
#include <render/vulkan/SwapChain.h>
#include <render/vulkan/LogicalDevice.h>

#include <string>
#include <filesystem>
#include <thread>
#include <stdio.h>
#include <assert.h>

namespace sr2 {
    u64 g_HostTimer = 1;
    u64 g_FrameTimer = 1;
    bool g_showFrameTime = false;

    GameEngine* GameEngine::instance = nullptr;

    GameEngine* GameEngine::Create(int argc, char** args) {
        // Originally this was created by now, but for now this is fine
        instance = new GameEngine(argc, args);

        msgMsgSource::registerSink(instance, 0x84, 0x80);
        instance->ChangeState(GAME_STATE::IN_MENU);

        if (!gfx::g_SomeViewport) gfx::g_SomeViewport = gfx::pipeline::Viewport;

        if (gfx::g_SomeViewport != gfx::pipeline::Viewport) {
            gfx::pipeline::ForceSetViewport(gfx::g_SomeViewport);
        }

        return GameEngine::instance;
    }

    void GameEngine::Terminate() {
        delete GameEngine::instance;
    }

    void GameEngine::Execute() {
        bool do_continue = true;
        while (do_continue) {
            do_continue = GameEngine::instance->Update();
        }
    }

    GameEngine* GameEngine::Instance() {
        return GameEngine::instance;
    }

    GameEngine::GameEngine(int argc, char** args) {
        std::string path = args[0];
        path = path.substr(0, path.find_last_of('/'));
        path = path.substr(0, path.find_last_of('\\'));
        std::filesystem::current_path(path);

        fsm = new gameFSM();
        frame_count = 0;
        should_pause = false;
        just_update = false;
        should_exit = false;
        m_currentFrame = nullptr;

        m_window = new utils::Window("Smuggler's Run 2", u32(gfx::pipeline::fWidth * debug_ui_scale), u32(gfx::pipeline::fHeight * debug_ui_scale));
        m_window->subscribe(&ioKeyboard::gKeyboard);
        if (!m_window->setOpen(true)) throw utils::Exception("Failed to open window");
        if (!initRendering(m_window)) throw utils::Exception("Failed to initialize renderer");
        if (!initDebugDrawing()) throw utils::Exception("Failed to initialize debug drawer");
        if (!initImGui()) throw utils::Exception("Failed to initialize ImGui");

        utils::Singleton<uiRenderer>::Create();
        if (!utils::Singleton<uiRenderer>::Get()->init(this)) throw utils::Exception("Failed to initialize ui renderer");
        utils::Singleton<srAudMgr>::Create(0xc, 1);
    }

    GameEngine::~GameEngine() {
        utils::Singleton<srAudMgr>::Destroy();
        utils::Singleton<uiRenderer>::Destroy();

        delete fsm;
        delete g_Archives;

        getLogicalDevice()->waitForIdle();
        shutdownRendering();

        m_window->unsubscribe(&ioKeyboard::gKeyboard);
        delete m_window;
    }

    void GameEngine::BeginFrame(bool doClear) {
        if (m_currentFrame) {
            // Not actually game logic, may need refactors to allow
            // code to independently begin/end frames? or refactor code that
            // independently begins/ends frames?
            return;
        }

        // IO::ioInput::Poll();
        gfx::pipeline::Manage();
        gfx::pipeline::BeginFrame();
        if (doClear) gfx::pipeline::Clear(3, gfx::g_clearColor, 1.0f, 0);
        g_HostTimer = timer::Ticks();

        m_currentFrame = getFrame();
        m_currentFrame->begin();
        m_currentFrame->setClearColor(0, render::vec4f(0.0f, 0.0f, 0.0f, 1.0f));
        m_currentFrame->setClearDepthStencil(1, 1.0f, 0);

        getDebugDraw()->begin(m_currentFrame->getSwapChainImageIndex());

        auto cb = m_currentFrame->getCommandBuffer();
        cb->beginRenderPass(
            getRenderPass(),
            m_currentFrame->getSwapChain(),
            m_currentFrame->getFramebuffer()
        );
        
        getImGui()->begin();

        // todo: after binding pipeline
        // auto screenSize = m_currentFrame->getSwapChain()->getExtent();
        // cb->setViewport(0, screenSize.height, screenSize.width, -f32(screenSize.height), 0, 1);
        // cb->setScissor(0, 0, screenSize.width, screenSize.height);
    }

    void GameEngine::EndFrame() {
        u64 t = timer::Ticks() - g_HostTimer;
        gfx::g_HostTime = f32(t) * 3.390842e-06f;

        if (g_showFrameTime) {
            // std::string tm = format("CPU=%5.2fms VU/GS=%5.2fms FRAME=%5.2fms", gfx::g_HostTime, gfx::g_DrawTime, gfx::g_FrameTime);
            // gfx::DrawFont(0x18, 0x18, tm.c_str(), 0x80ffffff);
        }
        
        auto dd = getDebugDraw();
        auto cb = m_currentFrame->getCommandBuffer();
        if (dd) {
            cb->endRenderPass();
            dd->end(cb);

            cb->beginRenderPass(
                getRenderPass(),
                m_currentFrame->getSwapChain(),
                m_currentFrame->getFramebuffer()
            );
            dd->draw(cb);
        }
        
        getImGui()->end(m_currentFrame);
        txtFontTex::renderAll();
        uiRenderer::draw(m_currentFrame);
        cb->endRenderPass();

        m_currentFrame->end();
        releaseFrame(m_currentFrame);
        m_currentFrame = nullptr;

        gfx::pipeline::EndFrame();
        t = timer::Ticks() - g_FrameTimer;
        gfx::g_FrameTime = f32(t) * 3.390842e-06f;
        g_FrameTimer = timer::Ticks();
    }

    bool GameEngine::Update() {
        if (!m_window->isOpen()) return false;

        m_window->pollEvents();

        BeginFrame(just_update);

        // FUN_002ed360(0, 1);
        // FUN_002ed360(1, 1);

        GameLoadingScreen::get()->FUN_001c57c0();

        GameState* state = fsm->current();
        if (just_update) {
            msgMsgSource::dispatch();
            state->Update();
        } else {

            state->PreUpdate();
            state->Input();
            msgMsgSource::dispatch();

            if (state->CanUpdateTime()) {
                datTimeManager::update();
                gfxTextureMovie::updateAll(datTimeManager::Seconds);
            }

            // There is also a lot of time tracking going on here, but
            // whether or not it has an effect on the game remains to
            // be seen.
            state->Update();
            state->Draw();
            state->PostUpdate();
        }

        EndFrame();

        if (should_pause) fsm->deferred_change(GAME_STATE::IN_GAME_PAUSED);

        if (just_update) {
            if (GameLoadingScreen::get()->FUN_001c5540()) {
                fsm->deferred_change(GAME_STATE::MENU_LOAD);
                just_update = false;
            }
        }

        fsm->update();
        return !state->Done() && !should_exit;
    }

    void GameEngine::ChangeState(GAME_STATE state) {
        fsm->change(state);
    }
    
    void GameEngine::onMessage(msgMessage* msg) {
        switch (msg->type) {
            case MSG_TYPE::RESET_GAME: {
                fsm->get(GAME_STATE::IN_GAME)->RequestReset();
                msgMsgSource::sendBroadcast(MSG_TYPE::UNK66);

                if (Aud3DObjectManager::isAlive()) Aud3DObjectManager::get()->FUN_001c1428();
                break;
            }
            case MSG_TYPE::ENTER_GAME: {
                fsm->deferred_change(GAME_STATE::IN_GAME);
                if (Aud3DObjectManager::isAlive()) Aud3DObjectManager::get()->FUN_001c14b0();
                break;
            }
            case MSG_TYPE::LOAD_GAME: {
                fsm->deferred_change(GAME_STATE::GAME_LOAD);
                break;
            }
            case MSG_TYPE::PAUSE_GAME: {
                fsm->get(GAME_STATE::IN_GAME)->method_0x58();
                fsm->deferred_change(GAME_STATE::IN_GAME_PAUSED);
                if (Aud3DObjectManager::isAlive()) Aud3DObjectManager::get()->FUN_001c1370();
                break;
            }
            case MSG_TYPE::QUIT_TO_MENU: {
                fsm->deferred_change(GAME_STATE::IN_MENU);
                break;
            }
            case MSG_TYPE::ENTER_MENU: {
                if (datArgParser::GetBooleanArgument("autoloadall")) {
                    if (Aud3DObjectManager::isAlive()) Aud3DObjectManager::get()->FUN_001c1370();
                    fsm->deferred_change(GAME_STATE::MENU_LOAD);
                } else {
                    just_update = true;
                    GameLoadingScreen::get()->FUN_001c54e8(9);
                    GameLoadingScreen::get()->FUN_001c54f0(0.5f);
                }

                break;
            }
            default: break;
        }
    }

    bool GameEngine::setupInstance(render::vulkan::Instance* instance) {
        instance->enableValidation();
        instance->subscribeLogger(this);
        return true;
    }

    void GameEngine::onLogMessage(utils::LOG_LEVEL level, const utils::String& scope, const utils::String& message) {
        if (level == utils::LOG_LEVEL::LOG_FATAL) {
            MessageBoxA(m_window->getHandle(), message.c_str(), scope.c_str(), MB_ICONEXCLAMATION);
        }

        printf("[%s]: %s\n", scope.c_str(), message.c_str());
        fflush(stdout);
        assert(level != utils::LOG_LEVEL::LOG_FATAL && level != utils::LOG_LEVEL::LOG_ERROR);
    }

    render::core::FrameContext* GameEngine::currentFrame() {
        return m_currentFrame;
    }
};