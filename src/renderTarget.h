// this part is code from article
// https://uysalaltas.github.io/2022/01/09/OpenGL_Imgui.html
//  ☀️
#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

// Wrapper to OpenGL Framebuffer and REnderBuffer objects
class RenderTarget {
 public:
  RenderTarget(float width, float height);
  ~RenderTarget();
  unsigned int getFrameTexture();
  void Rescale(float width, float height);
  void Bind() const;
  void Unbind() const;
  const float GetWidth() const;
  const float GetHeight() const;

 private:
  unsigned int fbo;
  unsigned int texture;
  unsigned int rbo;
  float rtWidth;
  float rtHeight;
};

#endif  // RENDER_TARGET_H
