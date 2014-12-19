#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include <unordered_map>
#include <string>
#include <utility>
namespace types {

    class ShaderProgram {
        public:
            struct UniformBlock {
                std::string uniformBlockName;
                GLubyte *dataPointer;
                GLint blockSize;
                GLuint UB;

                UniformBlock(const std::string &uniformBlockName, GLubyte *dataPointer, GLint blockSize, GLuint UB);
            };
        private:
            // stores uniform blocks shared between all shader programs, name - (bufferObject, UBO ID)
            static std::unordered_map<std::string, UniformBlock *> _uniformBlocks;

            unsigned int _programID;
            unsigned int _shaderCount;
            std::unordered_map<std::string, unsigned int> _uniformLoc;
            std::vector<Shader *> _attachedShaders;
        public:
            ShaderProgram(void);
            ~ShaderProgram(void);
            void attachShader(Shader *pShader);
            bool link() const;
            void use() const;
            void disable() const;
            unsigned int addUniform(const std::string &sUniformName);
            unsigned int getUniform(const std::string &sUniformName) const;
            unsigned int addUniformBlock(const std::string &sUniformBlockName);
            UniformBlock *getUniformBlock(const std::string &sUniformBlockName) const;
            void getUniformBlockIndexAndOffset(const std::string &uniformBlockName, const char *names[], GLuint *outIndices[], GLint *outOffset[], const unsigned int &count) const;

            /*
             * Validates the uniform name and location
             * and forwards the rvalue to a specific
             * overloaded function that calls:
             * glUniform{1|2|3|4}{f|i|ui} based on number of parameters
             * glUniform{2|3|4}fv for type glm::vec{2|3|4}
             * glUniformMatrix{2|3|4}fv for type glm::mat{2|3|4}
             * Method:    setUniform
             * FullName:  Types::ShaderProgram::setUniform
             * Access:    public
             * Parameter: const std::string & sUniformName
             * Parameter: T && value{1|2|3|4} -- rvalue to be forwarded to a specific overloaded function based on value type
             */
            template<typename T> void setUniform(const std::string &sUniformName, T &&value0) const;
            template<typename T> void setUniform(const std::string &sUniformName, T &&value0, T &&value1) const;
            template<typename T> void setUniform(const std::string &sUniformName, T &&value0, T &&value1, T &&value2) const;
            template<typename T> void setUniform(const std::string &sUniformName, T &&value0, T &&value1, T &&value2, T &&value3) const;

            void setUniform(unsigned int uniformLocation, const float &value0, const float &value1) const;
            void setUniform(unsigned int uniformLocation, const float &value0, const float &value1, const float &value2) const;
            void setUniform(unsigned int uniformLocation, const float &value0, const float &value1, const float &value2, const float &value3) const;

            void setUniform(unsigned int uniformLocation, const int &value0, const int &value1) const;
            void setUniform(unsigned int uniformLocation, const int &value0, const int &value1, const int &value2) const;
            void setUniform(unsigned int uniformLocation, const int &value0, const int &value1, const int &value2, const int &value3) const;

            void setUniform(unsigned int uniformLocation, const unsigned int &value0, const unsigned int &value1) const;
            void setUniform(unsigned int uniformLocation, const unsigned int &value0, const unsigned int &value1, const unsigned int &value2) const;
            void setUniform(unsigned int uniformLocation, const unsigned int &value0, const unsigned int &value1, const unsigned int &value2, const unsigned int &value3) const;

            void setUniform(unsigned int uniformLocation, const float &value0) const;
            void setUniform(unsigned int uniformLocation, const int &value0) const;
            void setUniform(unsigned int uniformLocation, const unsigned int &value0) const;

            void setUniform(unsigned int uniformLocation, const glm::mat4 &value0) const;
            void setUniform(unsigned int uniformLocation, const glm::mat3 &value0) const;
            void setUniform(unsigned int uniformLocation, const glm::mat2 &value0) const;

            void setUniform(unsigned int uniformLocation, const glm::vec4 &value0) const;
            void setUniform(unsigned int uniformLocation, const glm::vec3 &value0) const;
            void setUniform(unsigned int uniformLocation, const glm::vec2 &value0) const;
    };

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0) const
    {
        GLint uniformLocation = getUniform(sUniformName);

        if (uniformLocation == -1) { return; }

        setUniform(uniformLocation, std::forward<T>(value0));
    }

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0, T &&value1) const
    {
        GLint uniformLocation = getUniform(sUniformName);

        if (uniformLocation == -1) { return; }

        setUniform(uniformLocation, std::forward<T>(value0), std::forward<T>(value1));
    }

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0, T &&value1, T &&value2) const
    {
        GLint uniformLocation = getUniform(sUniformName);

        if (uniformLocation == -1) { return; }

        setUniform(uniformLocation, std::forward<T>(value0), std::forward<T>(value1), std::forward<T>(value2));
    }

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0, T &&value1, T &&value2, T &&value3) const
    {
        GLint uniformLocation = getUniform(sUniformName);

        if (uniformLocation == -1) { return; }

        setUniform(uniformLocation, std::forward<T>(value0), std::forward<T>(value1), std::forward<T>(value2), std::forward<T>(value3));
    }
}

