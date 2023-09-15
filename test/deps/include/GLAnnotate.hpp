#ifndef GLANNOTATE_HPP
#define GLANNOTATE_HPP

//POSSIBLE PLATFORMS
//@TODO: Add IOS & Android
#define GLANNOTATE_PLATFORM_OSX 1
#define GLANNOTATE_PLATFORM_LINUX 2

#if defined ( __APPLE__ )
#define GLANNOTATE_PLATFORM GLANNOTATE_PLATFORM_OSX
#elif __linux__
#define GLANNOTATE_PLATFORM GLANNOTATE_PLATFORM_LINUX
#endif

#ifndef __gl_h_
#if GLANNOTATE_PLATFORM == GLANNOTATE_PLATFORM_OSX
#include <OpenGL/OpenGL.h> //for CGL functions
//check if opengl 3+ is available
#if CGL_VERSION_1_3
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif //CGL_VERSION_1_3
#elif GLANNOTATE_PLATFORM == GLANNOTATE_PLATFORM_LINUX
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#undef GL_GLEXT_PROTOTYPES
#endif
#endif

//DEBUG
#if !defined(NDEBUG)
#define GLANNOTATE_DEBUG
#else
#undef GLANNOTATE_DEBUG
#endif

#ifdef GLANNOTATE_DEBUG
#define GLANNOTATE_ASSERT_NO_GL_ERROR(_func) do { _func; \
GLenum err = glGetError(); \
if(err != GL_NO_ERROR) \
{ \
switch(err) \
{ \
case GL_NO_ERROR: \
std::printf("%s line %i GL_NO_ERROR: No error has been recorded.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_ENUM: \
std::printf("%s line %i GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_VALUE: \
std::printf("%s line %i GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_OPERATION: \
std::printf("%s line %i GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_INVALID_FRAMEBUFFER_OPERATION: \
std::printf("%s line %i GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n", __FILE__, __LINE__);\
break; \
case GL_OUT_OF_MEMORY: \
std::printf("%s line %i GL_OUT_OF_MEMORY: There is not enough memory left to executeLua the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n", __FILE__, __LINE__);\
break; \
} \
exit(EXIT_FAILURE); \
} \
} while(false)
#else
#define GLANNOTATE_ASSERT_NO_GL_ERROR(_func) _func
#endif


// Settings
#define GLANNOTATE_MAX_MATRIXSTACK_DEPTH 64
#define GLANNOTATE_MAX_NAME_LENGTH 128
#define GLANNOTATE_MAX_BUFFERELEMENT_COUNT 16
#define GLANNOTATE_MAX_BUFFERLAYOUT_COUNT 16
#define GLANNOTATE_MAX_ACTIVE_ATTRIBUTES 16

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cassert>
#include <algorithm>

namespace gla
{
    using Float = float;
    const Float pi = 3.14159265359;
    const Float twoPi = pi * 2;

    namespace detail
    {
        //only for internal use to implement a simple matrix stack
        //these are by no means feature complete and literally only
        //implement what is needed right now
        struct Vector3
        {
            Float x, y, z;
        };

        struct Vector4
        {
            Float x, y, z, w;
        };

        inline Vector3 operator - (const Vector3 & _a, const Vector3 & _b)
        {
            return {_a.x - _b.x, _a.y - _b.y, _a.z - _b.z};
        }

        inline Float length(const Vector3 & _vec)
        {
            return std::sqrt(_vec.x * _vec.x + _vec.y * _vec.y + _vec.z * _vec.z);
        }

        inline Vector3 normalize(const Vector3 & _vec)
        {
            Float invLen = 1.0 / length(_vec);
            return {_vec.x * invLen, _vec.y * invLen, _vec.z * invLen};
        }

        inline Vector3 cross(const Vector3 & _a, const Vector3 & _b)
        {
            return {_a.y * _b.z - _a.z * _b.y,
                    _a.z * _b.x - _a.x * _b.z,
                    _a.x * _b.y - _a.y * _b.x};
        }

        inline Float dot(const Vector3 & _a, const Vector3 & _b)
        {
            return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z;
        }

        struct Matrix4
        {
            inline Matrix4()
            {
            }

            inline Matrix4(const Vector4 & _col0,
                           const Vector4 & _col1,
                           const Vector4 & _col2,
                           const Vector4 & _col3) :
                col0(_col0),
                col1(_col1),
                col2(_col2),
                col3(_col3)
            {

            }

            inline Matrix4(const Float * _data)
            {
                std::memcpy(&col0.x, _data, sizeof(Float) * 16);
            }

            inline Vector4 operator * (const Vector4 & _vec) const
            {
                return {_vec.x * col0.x + _vec.y * col1.x + col2.x * _vec.z + col3.x * _vec.w,
                        _vec.x * col0.y + _vec.y * col1.y + col2.y * _vec.z + col3.y * _vec.w,
                        _vec.x * col0.z + _vec.y * col1.z + col2.z * _vec.z + col3.z * _vec.w,
                        _vec.x * col0.w + _vec.y * col1.w + col2.w * _vec.z + col3.w * _vec.w
                       };
            }

            inline Matrix4 operator * (const Matrix4 & _mat) const
            {
                return
                {
                    *this * _mat.col0,
                    *this * _mat.col1,
                    *this * _mat.col2,
                    *this * _mat.col3
                };
            }

            inline static Matrix4 identity()
            {
                Matrix4 ret;

                ret.col0 = {1.0f, 0.0f, 0.0f, 0.0f};
                ret.col1 = {0.0f, 1.0f, 0.0f, 0.0f};
                ret.col2 = {0.0f, 0.0f, 1.0f, 0.0f};
                ret.col3 = {0.0f, 0.0f, 0.0f, 1.0f};

                return ret;
            }

            inline static Matrix4 translation(Float _x, Float _y, Float _z)
            {
                return Matrix4({1.0, 0.0, 0.0, 0.0},
                {0.0, 1.0, 0.0, 0.0},
                {0.0, 0.0, 1.0, 0.0},
                {_x, _y, _z, 1.0});
            }

            inline static Matrix4 rotationAxisAngle(Float _x, Float _y, Float _z, Float _radians)
            {
                Float c = std::cos(_radians);
                Float s = std::sin(_radians);
                Float C = Float(1.0) - c;
                Float xs = _x * s;
                Float ys = _y * s;
                Float zs = _z * s;
                Float xC = _x * C;
                Float yC = _y * C;
                Float zC = _z * C;
                Float xyC = _x * yC;
                Float yzC = _y * zC;
                Float zxC = _z * xC;

                return Matrix4(
                {_x * xC + c, xyC + zs, zxC - ys, 0.0},
                {xyC - zs, _y * yC + c, yzC + xs, 0.0},
                {zxC + ys, yzC - xs, _z * zC + c, 0.0},
                {0.0, 0.0, 0.0, 1.0}
                );
            }

            inline static Matrix4 scaling(Float _x, Float _y, Float _z)
            {
                return Matrix4(
                {_x, 0.0, 0.0, 0.0},
                {0.0, _y, 0.0, 0.0},
                {0.0, 0.0, _z, 0.0},
                {0.0, 0.0, 0.0, 1.0}
                );
            }

            inline static Matrix4 ortho(Float _left, Float _right, Float _bottom, Float _top, Float _near, Float _far)
            {
                Float a = 2.0 / (_right - _left);
                Float b = 2.0 / (_top - _bottom);
                Float c =  - 2.0 / (_far - _near);
                Float tx = -(_right + _left) / (_right - _left);
                Float ty = -(_top + _bottom) / (_top - _bottom);
                Float tz = -(_far + _near) / (_far - _near);

                return Matrix4(
                {a, 0.0, 0.0, 0.0},
                {0.0, b, 0.0, 0.0},
                {0.0, 0.0, c, 0.0},
                {tx, ty, tz, 1.0});
            }

            inline static Matrix4 frustum(Float _left, Float _right, Float _bottom, Float _top, Float _near, Float _far)
            {
                Float a = (2.0 * _near) / (_right - _left);
                Float b = (2.0 * _near) / (_top - _bottom);
                Float c = (_right + _left) / (_right - _left);
                Float d = (_top + _bottom) / (_top - _bottom);
                Float e = (-_far + _near) / (_far - _near);
                Float f = (-2.0 * _far * _near) / (_far - _near);

                return Matrix4(
                {a,   0.0, 0.0, 0.0},
                {0.0, b,   0.0, 0.0},
                {c,   d,   e,   -1.0},
                {0.0, 0.0, f,   0.0});
            }

            struct FrustumPlanes
            {
                Float v[6];
            };

            inline static FrustumPlanes frustumPlanesFromPerspective(Float _fovy, Float _aspect, Float _near, Float _far)
            {
                Float tanFovy = std::tan((_fovy * 0.5) * 0.017453293); //0.017453293 to convert to rads
                Float height = tanFovy * _near;
                Float width = height * _aspect;
                return { -width, width, -height, height, _near, _far};
            }

            inline static Matrix4 perspective(Float _fovy, Float _aspect, Float _near, Float _far)
            {
                auto fr = frustumPlanesFromPerspective(_fovy, _aspect, _near, _far);
                return frustum(fr.v[0], fr.v[1], fr.v[2], fr.v[3], fr.v[4], fr.v[5]);
            }

            inline static Matrix4 lookAt(const Vector3 & _eye, const Vector3 & _center, const Vector3 & _up)
            {
                Vector3 f = normalize(_center - _eye);
                Vector3 s = normalize(cross(f, _up));
                Vector3 u = cross(s, f);

                return Matrix4(
                {s.x, u.x, -f.x, 0.0},
                {s.y, u.y, -f.y, 0.0},
                {s.z, u.z, -f.z, 0.0},
                { -dot(s, _eye), -dot(u, _eye), dot(f, _eye), 1.0});
            }

            Vector4 col0, col1, col2, col3;
        };

        struct MatrixStack
        {
            MatrixStack() :
                index(0)
            {

            }

            void push(const Matrix4 & _mat)
            {
                assert(index < GLANNOTATE_MAX_MATRIXSTACK_DEPTH);
                stack[index++] = _mat;
            }

            const Matrix4 & pop()
            {
                return stack[--index];
            }

            Matrix4 stack[GLANNOTATE_MAX_MATRIXSTACK_DEPTH];
            std::size_t index;
        };

        struct AttributeValue
        {
            Vector4 value;
            std::size_t count; //1,2,3 or 4;
        };
    }

    struct BufferElement
    {
        inline BufferElement()
        {
            std::memset(name, 0, sizeof(name));
        }

        inline BufferElement(const char * _name, std::size_t _elementCount, GLenum _type) :
            elementCount(_elementCount),
            type(_type)
        {
            assert(std::strlen(_name) <= GLANNOTATE_MAX_NAME_LENGTH);
            std::strcpy(name, _name);
        }

        inline bool operator == (const BufferElement & _other) const
        {
            return std::strcmp(name, _other.name) == 0 && elementCount == _other.elementCount
                   && offset == _other.offset && stride == _other.stride && location == _other.location
                   && type == _other.type;
        }

        inline bool operator != (const BufferElement & _other) const
        {
            return !(*this == _other);
        }


        char name[GLANNOTATE_MAX_NAME_LENGTH];
        std::size_t elementCount;
        std::size_t offset;
        std::size_t stride;
        GLuint location;
        GLenum type;
    };

    struct BufferLayout
    {
        inline BufferLayout() :
            count(0)
        {

        }

        inline BufferLayout(std::initializer_list<BufferElement> _elements)
        {
            assert(_elements.size() <= GLANNOTATE_MAX_BUFFERELEMENT_COUNT);

            std::size_t off = 0;
            for (std::size_t i = 0; i < _elements.size(); i++)
            {
                auto & el = elements[i];
                el = BufferElement(*(_elements.begin() + i));
                el.offset = off;
                el.location = i;
                std::size_t bc = 1;
                switch (el.type)
                {
                    case GL_SHORT:
                    case GL_UNSIGNED_SHORT:
                        bc = 2;
                        break;
                    case GL_FLOAT:
                    case GL_INT:
                    case GL_UNSIGNED_INT:
                        bc = 4;
                        break;
                    case GL_DOUBLE:
                        bc = 8;
                        break;
                }
                off += el.elementCount * bc;
            }

            count = _elements.size();
            for (std::size_t i = 0; i < count; i++)
                elements[i].stride = off;
        }

        inline bool operator == (const BufferLayout & _other) const
        {
            if (count != _other.count)
                return false;

            for (std::size_t i = 0; i < count; i++)
            {
                if (elements[i] != _other.elements[i])
                    return false;
            }

            return true;
        }

        inline bool operator != (const BufferLayout & _other) const
        {
            return !(*this == _other);
        }

        BufferElement elements[GLANNOTATE_MAX_BUFFERELEMENT_COUNT];
        std::size_t count;
    };

    struct GLProgram
    {
        inline GLProgram() :
            handle(0)
        {
        }

        explicit operator bool() const
        {
            return handle != 0;
        }

        GLuint handle;
        std::size_t bufferLayoutIndex;
    };

    class GLAnnotate
    {
    public:

        inline GLAnnotate() :
            m_vertexData(nullptr)
        {

        }

        inline ~GLAnnotate()
        {
            //@TODO: Double check if I missed any cleanup
            if (m_vertexData)
            {
                std::free(m_vertexData);

                destroyProgram(m_defaultProg);
                destroyProgram(m_texProgram);

                for (auto & layout : m_bufferLayouts)
                {
                    glDeleteBuffers(1, &layout.vbo);
                    glDeleteVertexArrays(1, &layout.vao);
                }
            }
        }

        inline bool init()
        {
            m_currentTexture = m_activeAttributeCount = m_bufferLayoutCount = m_tpGeneration = 0;
            std::memset(m_errorBuffer, 0, sizeof(m_errorBuffer));

            const char * defVertexShader = "#version 150 \n"
                                           "uniform mat4 transProj;\n"
                                           "in vec3 vertex; \n"
                                           "in vec4 color; \n"
                                           "out vec4 col;\n"
                                           "void main() \n"
                                           "{ \n"
                                           "   col = color;\n"
                                           "   gl_Position = transProj * vec4(vertex, 1.0); \n"
                                           "} \n";

            const char * defFragmentShader = "#version 150 \n"
                                             "in vec4 col; \n"
                                             "out vec4 fragColor; \n"
                                             "void main() \n"
                                             "{ \n"
                                             "   fragColor = col; \n"
                                             "} \n";

            const char * defTexVertexShader = "#version 150 \n"
                                              "uniform mat4 transProj;\n"
                                              "in vec3 vertex; \n"
                                              "in vec4 color; \n"
                                              "in vec2 textureCoordinates;\n"
                                              "out vec4 col;\n"
                                              "out vec2 tc;\n"
                                              "void main() \n"
                                              "{ \n"
                                              "   col = color;\n"
                                              "   tc = textureCoordinates;\n"
                                              "   gl_Position = transProj * vec4(vertex, 1.0); \n"
                                              "} \n";

            const char * defTexFragmentShader = "#version 150 \n"
                                                "uniform sampler2D tex;\n"
                                                "in vec4 col; \n"
                                                "in vec2 tc; \n"
                                                "out vec4 fragColor; \n"
                                                "void main() \n"
                                                "{ \n"
                                                "   fragColor = texture(tex, tc) * col; \n"
                                                "   //fragColor = vec4(tc.x, tc.y, 0.0, 1.0) * col; \n"
                                                "} \n";

            BufferLayout layout =
            {
                {"vertex", 3, GL_FLOAT},
                {"color", 4, GL_FLOAT}
            };

            BufferLayout texLayout =
            {
                {"vertex", 3, GL_FLOAT},
                {"color", 4, GL_FLOAT},
                {"textureCoordinates", 2, GL_FLOAT}
            };

            m_defaultProg = createProgram(defVertexShader, defFragmentShader, layout);
            m_texProgram = createProgram(defTexVertexShader, defTexFragmentShader, texLayout);
            if (!m_defaultProg || !m_texProgram) return true;

            m_transform = detail::Matrix4::identity();
            m_projection = detail::Matrix4::identity();

            setColor(1.0f, 1.0f, 1.0f);

            //create the vao / vbo
            m_bufferLayouts[0].layout = layout;
            m_bufferLayouts[0].tpLoc = glGetUniformLocation(m_defaultProg.handle, "transProj");
            bool success = createVAO(layout, m_bufferLayouts[0].vao, m_bufferLayouts[0].vbo);
            if (!success) return true;

            m_bufferLayouts[1].layout = layout;
            m_bufferLayouts[1].tpLoc = glGetUniformLocation(m_texProgram.handle, "transProj");
            m_bufferLayouts[1].texLoc = glGetUniformLocation(m_texProgram.handle, "tex");
            success = createVAO(texLayout, m_bufferLayouts[1].vao, m_bufferLayouts[1].vbo);
            if (!success) return true;
            m_bufferLayoutCount = 2;

            m_vertexDataCapacity = 1024;
            m_vertexData = static_cast<Float *>(std::malloc(m_vertexDataCapacity * sizeof(Float)));
            m_vertexDataCount = 0;

            return false;
        }

        inline void setTransform(const Float * _mat)
        {
            m_transform = detail::Matrix4(_mat);
            m_tpGeneration++;
        }

        inline void setProjection(const Float * _mat)
        {
            m_projection = detail::Matrix4(_mat);
            m_tpGeneration++;
        }

        inline void setTransform(const detail::Matrix4 & _mat)
        {
            setTransform(&_mat.col0.x);
        }

        inline void setProjection(const detail::Matrix4 & _mat)
        {
            setProjection(&_mat.col0.x);
        }

        inline void setColor(Float _r, Float _g, Float _b, Float _a = 1.0f)
        {
            m_color[0] = _r;
            m_color[1] = _g;
            m_color[2] = _b;
            m_color[3] = _a;
            setAttribute("color", _r, _g, _b, _a);
        }

        inline const char * error() const
        {
            return m_errorBuffer;
        }

        inline void line(Float _x, Float _y, Float _x2, Float _y2)
        {
            begin(GL_LINES);
            vertex(_x, _y, 0);
            vertex(_x2, _y2, 0);
            end();
        }

        inline void line(Float _x, Float _y, Float _z, Float _x2, Float _y2, Float _z2)
        {
            begin(GL_LINES);
            vertex(_x, _y, _z);
            vertex(_x2, _y2, _z2);
            end();
        }

        inline void rect(Float _x, Float _y, Float _w, Float _h)
        {
            begin(GL_TRIANGLE_STRIP);
            vertex(_x, _y, 0);
            vertex(_x, _y + _h, 0);
            vertex(_x + _w, _y, 0);
            vertex(_x + _w, _y + _h, 0);
            end();
        }

        inline void texture(GLuint _textureHandle, Float _x, Float _y, Float _w, Float _h, bool _bFlipVertically = false)
        {
            m_currentTexture = _textureHandle;
            begin(GL_TRIANGLE_STRIP, m_texProgram);
            setAttribute("textureCoordinates", 0.0, _bFlipVertically ? 1.0 : 0.0);
            vertex(_x, _y, 0);
            setAttribute("textureCoordinates", 0.0, _bFlipVertically ? 0.0 : 1.0);
            vertex(_x, _y + _h, 0);
            setAttribute("textureCoordinates", 1.0, _bFlipVertically ? 1.0 : 0.0);
            vertex(_x + _w, _y, 0);
            setAttribute("textureCoordinates", 1.0, _bFlipVertically ? 0.0 : 1.0);
            vertex(_x + _w, _y + _h, 0);
            end();
        }

        inline void circle(Float _x, Float _y, Float _r, unsigned int _resolution = 40)
        {
            begin(GL_TRIANGLE_FAN);

            Float radStep = twoPi / static_cast<Float>(_resolution);
            Float currentStep;

            //generate the vertices
            vertex(_x, _y, 0);
            for (unsigned int i = 0; i <= _resolution; ++i)
            {
                currentStep = radStep * i;
                vertex(_x + std::cos(currentStep) * _r, _y + std::sin(currentStep) * _r, 0.0);
            }

            end();
        }

        inline void translate(Float _x, Float _y, Float _z)
        {
            setTransform(m_transform * detail::Matrix4::translation(_x, _y, _z));
        }

        inline void rotate(Float _radians, Float _x, Float _y, Float _z)
        {
            setTransform(m_transform * detail::Matrix4::rotationAxisAngle(_x, _y, _z, _radians));
        }

        inline void scale(Float _x, Float _y, Float _z)
        {
            setTransform(m_transform * detail::Matrix4::scaling(_x, _y, _z));
        }

        inline void ortho(Float _left, Float _right, Float _bottom, Float _top, Float _near, Float _far)
        {
            setProjection(detail::Matrix4::ortho(_left, _right, _bottom, _top, _near, _far));
        }

        inline void frustum(Float _left, Float _right, Float _bottom, Float _top, Float _near, Float _far)
        {
            setProjection(detail::Matrix4::frustum(_left, _right, _bottom, _top, _near, _far));
        }

        inline void perspective(Float _fovy, Float _aspect, Float _near, Float _far)
        {
            setProjection(detail::Matrix4::perspective(_fovy, _aspect, _near, _far));
        }

        inline void lookAt(Float _eyeX, Float _eyeY, Float _eyeZ,
                           Float _centerX, Float _centerY, Float _centerZ,
                           Float _upX, Float _upY, Float _upZ)
        {
            setTransform(detail::Matrix4::lookAt({_eyeX, _eyeY, _eyeZ}, {_centerX, _centerY, _centerZ}, {_upX, _upY, _upZ}));
        }

        inline void pushTransform()
        {
            m_transformStack.push(m_transform);
        }

        inline void popTransform()
        {
            m_transform = m_transformStack.pop();
            m_tpGeneration++;
        }

        inline void pushProjection()
        {
            m_projectionStack.push(m_projection);
        }

        inline void popProjection()
        {
            m_projection = m_projectionStack.pop();
            m_tpGeneration++;
        }

        inline void begin(GLenum _mode)
        {
            begin(_mode, m_defaultProg);
        }

        inline void begin(GLenum _mode, const GLProgram & _program)
        {
            m_vertexCount = 0;
            m_vertexDataCount = 0;
            m_activeAttributeCount = 0;
            m_currentProgram = _program;
            m_currentMode = _mode;
            setAttribute("color", m_color[0], m_color[1], m_color[2], m_color[3]);
        }

        inline void vertex(Float _x, Float _y, Float _z)
        {
            addVertexData({_x, _y, _z});

            for (std::size_t i = 0; i < m_activeAttributeCount; i++)
            {
                addVertexData(&m_activeAttributes[i].value.value.x, m_activeAttributes[i].value.count);
            }

            m_vertexCount++;
        }

        inline void end()
        {
            auto & bufferLayout = m_bufferLayouts[m_currentProgram.bufferLayoutIndex];
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, bufferLayout.vbo));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(Float) * m_vertexDataCount, m_vertexData, GL_DYNAMIC_DRAW));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBindVertexArray(bufferLayout.vao));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glUseProgram(m_currentProgram.handle));

            if (m_currentTexture)
            {
                GLANNOTATE_ASSERT_NO_GL_ERROR(glActiveTexture(GL_TEXTURE0));
                GLANNOTATE_ASSERT_NO_GL_ERROR(glBindTexture(GL_TEXTURE_2D, m_currentTexture));
                GLANNOTATE_ASSERT_NO_GL_ERROR(glUniform1i(bufferLayout.texLoc, 0));
            }

            if (m_tpGeneration != bufferLayout.tpGeneration)
            {
                bufferLayout.tpGeneration = m_tpGeneration;
                detail::Matrix4 tp = m_projection * m_transform;
                GLANNOTATE_ASSERT_NO_GL_ERROR(glUniformMatrix4fv(bufferLayout.tpLoc, 1, false, &tp.col0.x));
            }

            GLANNOTATE_ASSERT_NO_GL_ERROR(glDrawArrays(m_currentMode, 0, m_vertexCount));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBindVertexArray(NULL));

            m_currentTexture = 0;
        }

        inline void setAttribute(const char * _name, Float _value)
        {
            setAttributeImpl(_name, {{_value, 0, 0, 0}, 1});
        }

        inline void setAttribute(const char * _name, Float _x, Float _y)
        {
            setAttributeImpl(_name, {{_x, _y, 0, 0}, 2});
        }

        inline void setAttribute(const char * _name, Float _x, Float _y, Float _z)
        {
            setAttributeImpl(_name, {{_x, _y, _z, 0}, 3});
        }

        inline void setAttribute(const char * _name, Float _x, Float _y, Float _z, Float _w)
        {
            setAttributeImpl(_name, {{_x, _y, _z, _w}, 4});
        }

        inline GLProgram createProgram(const char * _vertexShader, const char * _fragmentShader, const BufferLayout & _layout)
        {
            GLProgram ret;

            bool bFreshLayout = true;
            std::size_t lidx = 0;
            for (; lidx < m_bufferLayoutCount; lidx++)
            {
                if (m_bufferLayouts[lidx].layout == _layout)
                    bFreshLayout = false;
            }

            if (bFreshLayout)
            {
                m_bufferLayouts[m_bufferLayoutCount++].layout = _layout;
                ret.bufferLayoutIndex = m_bufferLayoutCount - 1;
            }
            else
            {
                ret.bufferLayoutIndex = lidx;
            }

            GLuint vertexShader, fragmentShader;
            bool success = compileShader(_vertexShader, GL_VERTEX_SHADER, vertexShader);
            if (success)
            {
                success = compileShader(_fragmentShader, GL_FRAGMENT_SHADER, fragmentShader);
            }
            if (!success) return ret;

            GLuint program = glCreateProgram();

            GLANNOTATE_ASSERT_NO_GL_ERROR(glAttachShader(program, vertexShader));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glAttachShader(program, fragmentShader));

            for (std::size_t i = 0; i < _layout.count; i++)
            {
                GLANNOTATE_ASSERT_NO_GL_ERROR(glBindAttribLocation(program, _layout.elements[i].location, _layout.elements[i].name));
            }

            GLANNOTATE_ASSERT_NO_GL_ERROR(glLinkProgram(program));

            //check if we had success
            GLint state;
            GLANNOTATE_ASSERT_NO_GL_ERROR(glGetProgramiv(program, GL_LINK_STATUS, &state));

            if (state == GL_FALSE)
            {
                GLint infologLength;
                GLANNOTATE_ASSERT_NO_GL_ERROR(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength));
                GLANNOTATE_ASSERT_NO_GL_ERROR(glGetProgramInfoLog(program, std::min(infologLength, 1024), &infologLength, m_errorBuffer));
                success = false;
            }

            GLANNOTATE_ASSERT_NO_GL_ERROR(glDeleteShader(vertexShader));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glDeleteShader(fragmentShader));

            if (!success)
            {
                glDeleteProgram(program);
            }
            else
            {
                m_bufferLayouts[ret.bufferLayoutIndex].tpLoc = glGetUniformLocation(program, "transProj");
                ret.handle = program;
            }

            return ret;
        }

        void destroyProgram(const GLProgram & _program)
        {
            if (_program.handle)
                GLANNOTATE_ASSERT_NO_GL_ERROR(glDeleteProgram(_program.handle));
        }

    private:

        inline void addVertexData(const Float * _data, std::size_t _count)
        {
            assert(m_vertexDataCount < m_vertexDataCapacity);
            if ((int)m_vertexDataCapacity - (int)m_vertexDataCount < _count)
            {
                m_vertexDataCapacity = _count > m_vertexDataCapacity ? _count * 2 : m_vertexDataCapacity * 2;
                m_vertexData = static_cast<Float *>(std::realloc(m_vertexData, m_vertexDataCapacity * sizeof(Float)));
                //@TODO: Return error? some form of error handling? - . -
                assert(m_vertexData);
            }

            std::memcpy(m_vertexData + m_vertexDataCount, _data, _count * sizeof(Float));
            m_vertexDataCount += _count;
        }

        inline void addVertexData(const detail::Vector3 & _vec)
        {
            addVertexData(&_vec.x, 3);
        }

        inline void setAttributeImpl(const char * _name, const detail::AttributeValue & _val)
        {
            bool bFound = false;
            for (std::size_t idx = 0; idx < m_activeAttributeCount; ++idx)
            {
                if (std::strcmp(m_activeAttributes[idx].name, _name) == 0)
                {
                    m_activeAttributes[idx].value = _val;
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {
                m_activeAttributes[m_activeAttributeCount++] = NamedAttribute(_name, _val);
            }
        }

        inline bool compileShader(const char * _shaderCode, GLenum _shaderType, GLuint & _outHandle)
        {
            GLenum glHandle = glCreateShader(_shaderType);
            GLint len = (GLint)std::strlen(_shaderCode);
            GLANNOTATE_ASSERT_NO_GL_ERROR(glShaderSource(glHandle, 1, &_shaderCode, &len));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glCompileShader(glHandle));

            //check if the shader compiled
            GLint state;
            GLANNOTATE_ASSERT_NO_GL_ERROR(glGetShaderiv(glHandle, GL_COMPILE_STATUS, &state));
            if (state == GL_FALSE)
            {
                GLint infologLength;
                GLANNOTATE_ASSERT_NO_GL_ERROR(glGetShaderiv(glHandle, GL_INFO_LOG_LENGTH, &infologLength));

                GLANNOTATE_ASSERT_NO_GL_ERROR(glGetShaderInfoLog(glHandle, std::min(infologLength, 1024), &infologLength, m_errorBuffer));
                glDeleteShader(glHandle);
                return false;
            }
            else
            {
                _outHandle = glHandle;
                return true;
            }
        }

        inline bool createVAO(const BufferLayout & _layout, GLuint & _outVAO, GLuint & _outVBO)
        {
            GLANNOTATE_ASSERT_NO_GL_ERROR(glGenVertexArrays(1, &_outVAO));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glGenBuffers(1, &_outVBO));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBindVertexArray(_outVAO));
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, _outVBO));
            for (std::size_t i = 0; i < _layout.count; ++i)
            {
                GLANNOTATE_ASSERT_NO_GL_ERROR(glVertexAttribPointer(_layout.elements[i].location, _layout.elements[i].elementCount, GL_FLOAT, GL_FALSE, _layout.elements[i].stride, ((char *)_layout.elements[i].offset)));
                GLANNOTATE_ASSERT_NO_GL_ERROR(glEnableVertexAttribArray(_layout.elements[i].location));
            }
            GLANNOTATE_ASSERT_NO_GL_ERROR(glBindVertexArray(NULL));

            return true;
        }

        char m_errorBuffer[1024];
        GLProgram m_defaultProg;
        GLProgram m_texProgram;
        detail::Matrix4 m_transform;
        detail::Matrix4 m_projection;
        std::size_t m_tpGeneration;
        detail::MatrixStack m_transformStack;
        detail::MatrixStack m_projectionStack;
        Float m_color[4];

        struct BufferLayoutWithMesh
        {
            BufferLayout layout;
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint tpLoc = 0;
            GLuint texLoc = 0;
            GLuint tpGeneration = -1;
        };
        BufferLayoutWithMesh m_bufferLayouts[GLANNOTATE_MAX_BUFFERLAYOUT_COUNT];
        std::size_t m_bufferLayoutCount;
        Float * m_vertexData;
        std::size_t m_vertexDataCapacity;
        std::size_t m_vertexCount;
        std::size_t m_vertexDataCount;
        GLProgram m_currentProgram;
        GLenum m_currentMode;
        GLuint m_currentTexture;

        //attribute things
        struct NamedAttribute
        {
            NamedAttribute()
            {

            }

            NamedAttribute(const char * _name, const detail::AttributeValue & _val) :
                value(_val)
            {
                std::strcpy(name, _name);
            }

            char name[GLANNOTATE_MAX_NAME_LENGTH];
            detail::AttributeValue value;
        };

        NamedAttribute m_activeAttributes[GLANNOTATE_MAX_ACTIVE_ATTRIBUTES];
        std::size_t m_activeAttributeCount;
    };
}

#endif //GLANNOTATE_HPP