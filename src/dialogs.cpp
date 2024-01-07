//  ☀️
#include "dialogs.h"

ImGuiMessageBox::ImGuiMessageBox(){};

ImGuiMessageBox::~ImGuiMessageBox(){};

void ImGuiMessageBox::show(const std::string& title,
            const std::string& message,
            const std::function<void()>& OkFun,
            const std::function<void()>& CancelFun) {
    title_ = title;
    message_ = message;
    OkFun_ = OkFun;
    CancelFun_ = CancelFun;

    ImGui::OpenPopup(title_.c_str());
}
void ImGuiMessageBox::process() {
    if (ImGui::BeginPopupModal(title_.c_str(), NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text(message_.c_str());
      if (ImGui::Button("OK", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        if (OkFun_) {
          std::cout << "ok function called" << std::endl;
          OkFun_();
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        if (CancelFun_) {
          std::cout << "cancel function called" << std::endl;
          CancelFun_();
        }
      }
      ImGui::EndPopup();
    }
}

ImGuiInputBox::ImGuiInputBox() : ImGuiMessageBox(){}

ImGuiInputBox::~ImGuiInputBox(){}

void ImGuiInputBox::process() {
    if (ImGui::BeginPopupModal(title_.c_str(), NULL,
                               ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::InputText(message_.c_str(), text_buf_, IM_ARRAYSIZE(text_buf_));
      if (ImGui::Button("OK", ImVec2(120, 0))) {
        std::cout << "ok pressed, text is " << text_buf_ << std::endl;
        ImGui::CloseCurrentPopup();
        if (OkFun_) {
          std::cout << "ok function called" << std::endl;
          OkFun_();
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel", ImVec2(120, 0))) {
        ImGui::CloseCurrentPopup();
        if (CancelFun_) {
          std::cout << "cancel function called" << std::endl;
          CancelFun_();
        }
      }
      ImGui::EndPopup();
    }
};
const char* ImGuiInputBox::getText() { 
	return text_buf_;
}

