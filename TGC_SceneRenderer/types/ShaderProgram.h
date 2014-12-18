#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include <unordered_map>
#include <string>
namespace types {

    class ShaderProgram {
        private:
            unsigned int _programID;
            unsigned int _shaderCount;
            std::unordered_map<std::string, unsigned int> _uniformLoc;
            std::unordered_map<std::string, GLubyte *> _uniformBlocks;
            std::vector<Shader *> _attachedShaders;
        public:
            ShaderProgram(void);
            ~ShaderProgram(void);
            void attachShader(Shader *pShader);
            bool link() const;
            void use() const;
            void disable() const;
            unsigned int getUniform(const std::string &sUniformName) const;
            GLubyte *getUniformBlock(const std::string &sUniformBlockName) const;
            unsigned int addUniform(const std::string &sUniformName);
            unsigned int addUniformBlock(const std::string &sUniformBlockName);

            /*
             * Method:    setUniformBlock
             * FullName:  types::ShaderProgram::setUniformBlock
             * Access:    public
             * Parameter: const std::string & sUniformBlockName
             * Parameter: const char * names[]
             * Parameter: const unsigned int & namesCount
             * Parameter: T & value
             * Parameter: const size_t & valueSize
             */
            template<typename T> void setUniformBlock(const std::string &sUniformBlockName, const char *names[], const unsigned int &namesCount, T &value, const size_t &valueSize) const;
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

            void setUniform(unsigned int unfrLoc, const float &value0, const float &value1) const;
            void setUniform(unsigned int unfrLoc, const float &value0, const float &value1, const float &value2) const;
            void setUniform(unsigned int unfrLoc, const float &value0, const float &value1, const float &value2, const float &value3) const;

            void setUniform(unsigned int unfrLoc, const int &value0, const int &value1) const;
            void setUniform(unsigned int unfrLoc, const int &value0, const int &value1, const int &value2) const;
            void setUniform(unsigned int unfrLoc, const int &value0, const int &value1, const int &value2, const int &value3) const;

            void setUniform(unsigned int unfrLoc, const unsigned int &value0, const unsigned int &value1) const;
            void setUniform(unsigned int unfrLoc, const unsigned int &value0, const unsigned int &value1, const unsigned int &value2) const;
            void setUniform(unsigned int unfrLoc, const unsigned int &value0, const unsigned int &value1, const unsigned int &value2, const unsigned int &value3) const;

            void setUniform(unsigned int unfrLoc, const float &value0) const;
            void setUniform(unsigned int unfrLoc, const int &value0) const;
            void setUniform(unsigned int unfrLoc, const unsigned int &value0) const;

            void setUniform(unsigned int unfrLoc, const glm::mat4 &value0) const;
            void setUniform(unsigned int unfrLoc, const glm::mat3 &value0) const;
            void setUniform(unsigned int unfrLoc, const glm::mat2 &value0) const;

            void setUniform(unsigned int unfrLoc, const glm::vec4 &value0) const;
            void setUniform(unsigned int unfrLoc, const glm::vec3 &value0) const;
            void setUniform(unsigned int unfrLoc, const glm::vec2 &value0) const;
    };

    template<typename T>
    void types::ShaderProgram::setUniformBlock(const std::string &sUniformBlockName, const char *names[], const unsigned int &namesCount, T &value, const size_t &valueSize) const
    {
        // Get uniform block buffer pointer
        GLubyte *pBlockBuffer = this->getUniformBlock(sUniformBlockName);

        if (!pBlockBuffer) { return; }

        // Gets index positions and offsets
        GLuint indices[namesCount];
        glGetUniformIndices(this->_programID, namesCount, names, indices);
        GLuint offset[namesCount];
        glGetActiveUniformsiv(this->_programID, namesCount, indices, GL_UNIFORM_OFFSET, offset);

        // Copy values to the uniform buffer locations
        for (unsigned int i = 0; i < namesCount; i++) {
            memcpy(pBlockBuffer + offset[i], T, valueSize);
        }
    }


    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0) const
    {
        GLint unfrLoc = getUniform(sUniformName);

        if (unfrLoc == -1) { return; }

        setUniform(unfrLoc, std::forward<T>(value0));
    }

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0, T &&value1) const
    {
        GLint unfrLoc = getUniform(sUniformName);

        if (unfrLoc == -1) { return; }

        setUniform(unfrLoc, std::forward<T>(value0), std::forward<T>(value1));
    }

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0, T &&value1, T &&value2) const
    {
        GLint unfrLoc = getUniform(sUniformName);

        if (unfrLoc == -1) { return; }

        setUniform(unfrLoc, std::forward<T>(value0), std::forward<T>(value1), std::forward<T>(value2));
    }

    template<typename T>
    void types::ShaderProgram::setUniform(const std::string &sUniformName, T &&value0, T &&value1, T &&value2, T &&value3) const
    {
        GLint unfrLoc = getUniform(sUniformName);

        if (unfrLoc == -1) { return; }

        setUniform(unfrLoc, std::forward<T>(value0), std::forward<T>(value1), std::forward<T>(value2), std::forward<T>(value3));
    }
}

