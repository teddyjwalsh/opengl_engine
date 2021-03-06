#include "tgl/ray_bounce.h"

namespace tgl
{

RayBounce::RayBounce():
    swapper(0)
{
}

RayBounce::~RayBounce()
{
}

void RayBounce::init()
{
    shadow_map_size = pow(2, 12);
    // The framebuffer, which regroups 0, 1,
    // or more textures, and 0 or 1 depth buffer.
    // GLuint FramebufferName = 0;
    glGenFramebuffers(1, &FramebufferName);

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // The texture we're going to render to
    glGenTextures(1, &depthTexture);

    // "Bind" the newly created texture :
    // all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 shadow_map_size,
                 shadow_map_size,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH_COMPONENT,
                          shadow_map_size,
                          shadow_map_size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              depthrenderbuffer);

    // Set "depthTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthTexture, 0);

    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);  // "1" is the size of DrawBuffers
    glGenFramebuffers(1, &FramebufferName2);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName2);

    // The texture we're going to render to
    glGenTextures(1, &depthTexture2);

    // "Bind" the newly created texture :
    //  all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, depthTexture2);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 shadow_map_size,
                 shadow_map_size,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 0);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // The depth buffer
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          GL_DEPTH_COMPONENT,
                          shadow_map_size,
                          shadow_map_size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER,
                              depthrenderbuffer);

    // Set "depthTexture2" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER,
                         GL_COLOR_ATTACHMENT0,
                         depthTexture2,
                         0);

    // Set the list of draw buffers.
    GLenum DrawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers2);  // "1" is the size of DrawBuffers



    v_shader = new Shader("content/shaders/ray_bounce_vertex.glsl",
                               GL_VERTEX_SHADER);
    f_shader = new Shader("content/shaders/ray_bounce_fragment.glsl",
                               GL_FRAGMENT_SHADER);

    mat = new Material();
    mat->add_shader(v_shader);
    mat->add_shader(f_shader);
    mat->link_shader();
}

void RayBounce::set_up()
{
    glm::vec3 lightInvDir = glm::vec3(0, 200, 0);
    // Compute the MVP matrix from the light's point of view
    glm::mat4 depthProjectionMatrix =
        glm::ortho<float>(-100, 100, -100, 100, 0, 1000);
    glm::mat4 depthViewMatrix =
        glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthMVP =
        depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

    GLuint depthMatrixID =
        glGetUniformLocation(mat->get_shader_program(), "depthMVP");
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
}

void RayBounce::swap_buffers()
{
    swapper = (swapper + 1) % 2;
}

GLuint RayBounce::get_framebuffer()
{
    if (swapper == 0)
    {
        return FramebufferName;
    }
    else
    {
        return FramebufferName2;
    }
}

GLuint RayBounce::get_texture()
{
    if (swapper == 0)
    {
        return depthTexture2;
    }
    else
    {
        return depthTexture;
    }
}

}  // namespace tgl
