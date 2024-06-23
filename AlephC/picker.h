#ifndef PICKER_H
#define PICKER_H

#include <glad.h>
#include <iostream>


class MousePicking
{
public:
    MousePicking() {}

    ~MousePicking()
    {
        if (FBO != 0) {
            glDeleteFramebuffers(1, &FBO);
        }

        if (texture != 0) {
            glDeleteTextures(1, &texture);
        }

        if (depthTexture != 0) {
            glDeleteTextures(1, &depthTexture);
        }
    }

    void Init(unsigned int win_width, unsigned int win_height)
    {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        // Create the texture object for the primitive information buffer
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, win_width, win_height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        // Create the texture object for the depth buffer
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, win_width, win_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

        // Verify that the FBO is correct
        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            printf("FB error, status: 0x%x\n", Status);
        }

        // Restore the default framebuffer
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void enable_writing()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);   
    }

    void disable_writing()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    struct PixelInfo
    {
        unsigned int ObjectID = 0;
        unsigned int DrawID = 0;
        unsigned int PrimID = 0;

        void Print()
        {
            printf("Object %d draw %d prim %d\n", ObjectID, DrawID, PrimID);
        }
    };

    PixelInfo ReadPixel(unsigned int x, unsigned int y)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);

        glReadBuffer(GL_COLOR_ATTACHMENT0);

        PixelInfo Pixel;
        glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &Pixel);

        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        return Pixel;
    }

private:
    unsigned int FBO = 0;
    unsigned int texture = 0;
    unsigned int depthTexture = 0;
};

#endif