//  ☀️
#ifndef DIALOGS_H
#define DIALOGS_H
#include <functional>
#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"

class ImGuiMessageBox {
 public:
  ImGuiMessageBox();
  virtual ~ImGuiMessageBox();

  void show(const std::string& title,
            const std::string& message,
            const std::function<void()>& OkFun = {},
            const std::function<void()>& CancelFun = {});
  virtual void process();
 protected:
  std::string message_{"Message"};
  std::string title_{"Title"};
  std::function<void()> OkFun_{};
  std::function<void()> CancelFun_{};
};

class ImGuiInputBox : public ImGuiMessageBox {
 public:
  ImGuiInputBox();
  virtual ~ImGuiInputBox();
  virtual void process() override;
  const char* getText();
 protected:
  char text_buf_[256] = "/data/code/diplom2/1.2d3d";
};

#endif //DIALOGS_H
