#include <libsr2/frontend/srfeMovieBase.h>
#include <libsr2/frontend/srfeTitlescreen.h>
#include <libsr2/frontend/srfeCredits.h>
#include <libsr2/utilities/datArgParser.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/managers/audManager.h>
#include <libsr2/io/stream.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Variable.h>
#include <libsr2/gfx/gfx.h>

namespace sr2 {
    bool srfeMovieBase::ShowSubtitles = true;

    u32 extractCsvStrings(const char* line, u32 maxStrs, u32 maxStrLen, char* str0, char* str1, char* str2, char* str3, char* str4) {
        char* strs[] = { str0, str1, str2, str3, str4 };
        u32 strCount = 0;

        for (;strCount < maxStrs;strCount++) {
            char curChar = *line;
            char* cString = strs[strCount];
            u32 cStringLen = 0;
            bool inQuote = false;

            if (curChar == '\0') cString = cString + cStringLen;
            else if (curChar != '\n' && curChar != '\r' && curChar != ',') {
                do {
                    if (curChar == '\"') inQuote = !inQuote;
                    else if (cStringLen < maxStrLen - 1) {
                        cString[cStringLen] = curChar;
                        cStringLen++;
                    }

                    line++;
                    curChar = *line;

                    if (curChar == '\0' || curChar == '\n' || curChar == '\r') break;
                } while ((inQuote) || (curChar != ','));

                cString = cString + cStringLen;
            }

            *cString = '\0';
            if (*line != ',') break;
            line++;
        }

        return strCount;
    }

    srfeMovieBase::srfeMovieBase(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
        field_0xb4 = 0;
        field_0xb8 = 0.0f;
        field_0xbc = 0;
        field_0x408 = 0;
        field_0x40c = 0;
        field_0x48 = 0;

        if (datArgParser::GetBooleanArgument("nofmv")) {
            field_0x408 = 1;
        }

        setShouldLoad(false);
        m_master->setShouldLoad(false);

        m_subtitleData.isLoaded = false;
        m_subtitleData.currentIdx = 0;
        m_subtitleData.field_0x8 = 0;
        m_subtitleData.color = 0;

        for (u32 i = 0;i < 40;i++) {
            m_subtitleData.subtitles[i].timeOffset = 100000.0f;
            m_subtitleData.subtitles[i].langTexts[0] = nullptr;
            m_subtitleData.subtitles[i].langTexts[1] = nullptr;
            m_subtitleData.subtitles[i].langTexts[2] = nullptr;
            m_subtitleData.subtitles[i].langTexts[3] = nullptr;
        }
    }

    srfeMovieBase::~srfeMovieBase() {
        deinitScreen();
        for (u32 i = 0;i < 40;i++) {
            if (m_subtitleData.subtitles[i].langTexts[0]) delete [] m_subtitleData.subtitles[i].langTexts[0];
            if (m_subtitleData.subtitles[i].langTexts[1]) delete [] m_subtitleData.subtitles[i].langTexts[1];
            if (m_subtitleData.subtitles[i].langTexts[2]) delete [] m_subtitleData.subtitles[i].langTexts[2];
            if (m_subtitleData.subtitles[i].langTexts[3]) delete [] m_subtitleData.subtitles[i].langTexts[3];
        }
    }

    void srfeMovieBase::draw() {
        ui2Screen::draw();

        if (!m_isActive) return;
        switch (field_0x40c) {
            case 1: {
                FUN_0011db18();
                break;
            }
            case 2: {
                FUN_0011db70();
                break;
            }
            case 3: {
                FUN_0011dbc0();
                break;
            }
            case 5: {
                FUN_0011dd08();
                break;
            }
            case 6: {
                FUN_0011ddb8();
                break;
            }
        }
    }

    void srfeMovieBase::setActive(bool isActive) {
        bool wasActive = m_isActive;
        ui2Screen::setActive(isActive);

        if (wasActive == isActive || !isActive || !m_isInitialized) return;

        m_input->FUN_001fee50();

        if (!field_0x408) set0x40c();
        else interceptEvent(nullptr, WidgetEventType::UNK63, nullptr);
    }

    const char* srfeMovieBase::getType() const {
        return "srfeMovieBase";
    }

    bool srfeMovieBase::isA(const char* type) const {
        if (strcmp("srfeMovieBase", type) == 0) return true;
        return srui2SRScreen::isA(type);
    }

    void srfeMovieBase::initScreen() {
        srui2SRScreen::initScreen();

        char nameBuf[100] = { 0 };
        snprintf(nameBuf, 100, "%s_subTitleText", getName());
        m_subtitleText = new ui2DynamicText(nameBuf, 64, gfx::pipeline::iHeight - 64, 0, "", 0x100, nullptr);
        
        snprintf(nameBuf, 100, "%s_movie", getName());
        m_movie = new ui2Movie(nameBuf, nullptr, 1, 0, 0, 0, nullptr, nullptr);
        m_movie->FUN_001f3350(1);
        m_movie->FUN_001f3328(0);
        m_movie->addListener(this, WidgetEventType::UNK63, (WidgetEventCallback)&srfeMovieBase::interceptEvent);
        m_movie->addListener(this, WidgetEventType::UNK64, (WidgetEventCallback)&srfeMovieBase::interceptEvent);
        m_movie->setActive(false);
        
        snprintf(nameBuf, 100, "%s_timer", getName());
        m_timer = new ui2Timer(nameBuf, 0.0f, false, false, nullptr);
        m_timer->addListener(this, WidgetEventType::TimerFinished, (WidgetEventCallback)&srfeMovieBase::interceptEvent);
        
        snprintf(nameBuf, 100, "%s_subTimer", getName());
        m_subTimer = new ui2Timer(nameBuf, 0.0f, true, false, nullptr);
        m_subTimer->addListener(this, WidgetEventType::TimerFinished, (WidgetEventCallback)&srfeMovieBase::interceptEvent);

        m_input->addListener(this, WidgetEventType::StartPressed, (WidgetEventCallback)&srfeMovieBase::interceptEvent);
        m_input->addListener(this, WidgetEventType::SelectPressed, (WidgetEventCallback)&srfeMovieBase::interceptEvent);
        m_input->addListener(this, WidgetEventType::AcceptPressed, (WidgetEventCallback)&srfeMovieBase::interceptEvent);
    }

    void srfeMovieBase::deinitScreen() {
        m_movie.release();
        m_timer.release();
        m_subTimer.release();
        m_subtitleText.release();
    }
    
    void srfeMovieBase::setNextScreenName(const char* p1) {
        m_nextScreenName.set(p1);
    }
    
    void srfeMovieBase::set0xb8(f32 p1) {
        field_0xb8 = p1;
    }
    
    void srfeMovieBase::set0xbc(undefined4 p1) {
        field_0xbc = p1;
    }
    
    void srfeMovieBase::set0x40c() {
        field_0x40c = 1;
    }

    void srfeMovieBase::FUN_0011db18() {
        if (!field_0x408) {                                   
            // FUN_00207888(0x27);
            // FUN_00207888(0x28);
            // FUN_00207888(0x3d);
            // FUN_00207380();
        }

        field_0x40c = 2;
    }

    void srfeMovieBase::FUN_0011db70() {
        if (!field_0x408) {
            // if (FUN_002073a8() != 1) return;
            // FUN_00207418();
        }

        field_0x40c = 3;
    }

    void srfeMovieBase::FUN_0011dbc0() {
        if (!field_0x408) {
            m_movie->setActive(true);
            // m_movie->FUN_001f31d0();
        }

        switch (field_0xbc) {
            case 0: {
                m_timer->setDuration(field_0xb8);
                break;
            }
            case 1: {
                m_timer->setDuration(1.0f);
                break;
            }
            case 2: {
                m_timer->setDuration(0.0f);
                break;
            }
            default: break;
        }

        m_timer->startTimer();
        field_0xb4 = 0;

        if (m_subtitleData.isLoaded) {
            m_subTimer->setDuration(m_subtitleData.subtitles[0].timeOffset);
            m_subTimer->startTimer();
            m_subtitleText->method_0x148("");
        }

        m_subtitleData.currentIdx = 0;
        field_0x40c = 4;
    }

    void srfeMovieBase::FUN_0011dd08() {
        if (field_0x408) {
            field_0x40c = 6;
            return;
        }

        // m_movie->FUN_001f3210();
        m_movie->setActive(false);

        if (!field_0xbc) {
            const char* name = m_nextScreenName.get();
            if (name && name[0] && !srfeMovieBase::isWidgetAMovie(name)) {
                // FUN_002074a0();
                field_0x40c = 6;
                return;
            }
        }

        // if (field_0xbc == 1 || field_0xbc == 3) FUN_002074a0();
        field_0x40c = 6;
    }

    void srfeMovieBase::FUN_0011ddb8() {
        const char* name = m_nextScreenName.get();
        if (!field_0x408) {
            if (!field_0xbc) {
                if (!name || !name[0] || srfeMovieBase::isWidgetAMovie(name)) {
                    if (field_0xbc == 1 || field_0xbc == 2) {
                        audManager::playSound(0x27);
                        audManager::playSound(0x28);
                    }
                } else {
                    audManager::playSound(0x27);
                    audManager::playSound(0x28);
                }
            } else {
                if (field_0xbc == 1 || field_0xbc == 2) {
                    audManager::playSound(0x27);
                    audManager::playSound(0x28);
                }
            }
        }

        if ((field_0xbc - 1) < 2 || (name && name[0] != '\0')) {
            WidgetRef<ui2Screen> scr;

            if (field_0xbc == 1) name = "Theater";
            else if (field_0xbc == 2) name = "Titlescreen";
            
            scr = ui2Base::getGlobalMaster()->findWidget(name, "ui2Screen").cast<ui2Screen>();
            if (scr) {
                if (field_0xbc == 0) scr->method_0xe8(1);
                else scr->method_0xa8(1);
            }
        } else {
            WidgetRef<ui2Variable> quitfe = ui2Base::getGlobalMaster()->findWidget("quitfe_var", "ui2Variable").cast<ui2Variable>();
            // if (quitfe) quitfe->FUN_002021f0(1);
        }
        
        field_0x40c = 0;
        setActive(false);
    }

    void srfeMovieBase::FUN_0011e130() {
        if (m_subtitleData.isLoaded) {
            m_subTimer->stopTimer();
            m_subtitleText->method_0x148("");
        }

        field_0x40c = 5;

        m_movie->stopMovie();
    }

    void srfeMovieBase::interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        switch (event) {
            case WidgetEventType::TimerFinished: {
                if (source == m_timer) {
                    field_0xb4 = 1;
                    return;
                }

                if (!m_subtitleData.isLoaded) return;

                
                LANGUAGE lang;
                u32 langFlags;
                ui2Base::getGlobalMaster()->getLangInfo(&lang, &langFlags);

                i32 textIdx = -1;
                if (lang == LANGUAGE::English) textIdx = 0;
                else if (lang == LANGUAGE::French) textIdx = 1;
                else if (lang == LANGUAGE::German) textIdx = 2;
                else if (lang == LANGUAGE::Italian) textIdx = 3;

                if (textIdx >= 0) {
                    m_subtitleText->method_0x148(m_subtitleData.subtitles[m_subtitleData.currentIdx].langTexts[textIdx]);
                }

                m_subTimer->setDuration(m_subtitleData.subtitles[m_subtitleData.currentIdx].timeOffset);
                m_subtitleData.currentIdx++;

                break;
            }
            case WidgetEventType::UNK63:
            case WidgetEventType::UNK64: {
                FUN_0011e130();
                break;
            }
            case WidgetEventType::StartPressed:
            case WidgetEventType::SelectPressed:
            case WidgetEventType::AcceptPressed: {
                if (field_0x40c != 4) return;

                if (field_0xb8 == 0.0f) {
                    if (field_0xbc != 0 && field_0xb4 == 0) return;
                } else if (field_0xb4 == 0) return;

                if (field_0xbc == 2) {
                    srfeTitlescreen::FUN_00117800(1);
                }

                FUN_0011e130();
                break;
            }
        }
    }

    void srfeMovieBase::loadSubtitles(const char* path, i32 p2, u32 color) {
        if (!srfeMovieBase::ShowSubtitles) return;

        m_subtitleData.field_0x8 = p2;
        m_subtitleData.color = color;

        m_subtitleText->setAlignment(0);
        m_subtitleText->method_0xe8(64, gfx::pipeline::iHeight - p2);
        m_subtitleText->setBounds(gfx::pipeline::iWidth - 128, 64);
        m_subtitleText->setColorU32(color);
        m_subtitleText->method_0x170("uiMedL_shadow");

        Stream* fp = datAssetManager::open("tune/subtitles", path, "csv", 0, true);
        if (!fp) return;

        m_subtitleData.isLoaded = true;
        char line[1280] = { 0 };
        fgets(line, 1280, fp->getRaw());

        u32 subtitleIdx = 0;
        f32 timestamp = 0.0f;
        f32 offset = 0.0f;
        char english[256] = { 0 };
        char french[256] = { 0 };
        char german[256] = { 0 };
        char italian[256] = { 0 };
        char timecode[256] = { 0 };

        while (true) {
            if (!fgets(line, 1280, fp->getRaw())) break;
            timecode[0] = '\0';
            english[0] = '\0';
            french[0] = '\0';
            german[0] = '\0';
            italian[0] = '\0';

            extractCsvStrings(line, 4, 256, timecode, english, french, german, italian);

            f32 timeOffsetInSeconds = 0.0f;
            char* tmp = strchr(timecode, ':');
            if (!tmp) timeOffsetInSeconds = atof(timecode);
            else {
                if (strlen(timecode) != 11) break;
                tmp[3] = 0;
                f32 minutes = atof(tmp + 1);
                tmp[6] = '.';
                f32 seconds = atof(tmp + 4);

                timeOffsetInSeconds = (minutes * 60.0f) + seconds;
            }

            if (strcmp(english, "time correction")) {
                timeOffsetInSeconds += timestamp;
                m_subtitleData.subtitles[subtitleIdx].timeOffset = timeOffsetInSeconds - offset;
                m_subtitleData.subtitles[subtitleIdx].langTexts[0] = duplicateString(english);
                m_subtitleData.subtitles[subtitleIdx].langTexts[1] = duplicateString(french);
                m_subtitleData.subtitles[subtitleIdx].langTexts[2] = duplicateString(german);
                m_subtitleData.subtitles[subtitleIdx].langTexts[3] = duplicateString(italian);

                subtitleIdx++;
                offset = timeOffsetInSeconds;
            } else {
                timestamp += timeOffsetInSeconds;
            }
        }

        delete fp;
    }

    bool srfeMovieBase::isWidgetAMovie(const char* someWidgetName) {
        ui::BaseRef w = ui2Base::getGlobalMaster()->findWidget(someWidgetName, "srfeMovieBase");
        if (!w) return false;

        return true;
    }
};