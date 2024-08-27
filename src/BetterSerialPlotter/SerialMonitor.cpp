#include <BetterSerialPlotter/SerialMonitor.hpp>
#include <BetterSerialPlotter/BSP.hpp>
using namespace std;

namespace bsp{

SerialMonitor::SerialMonitor(BSP* gui_): Widget(gui_){}

void SerialMonitor::render(){
    constexpr ImGuiWindowFlags serial_monitor_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::Checkbox("Auto-Scroll",&auto_scroll);
    char buf[1024]{};
    bool textInputRecieved = false;
    if (ImGui::InputText("Press enter to send", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        //enter pressed grab data
        string text = buf;
        textInputRecieved = true;
    }
    ImGui::BeginChild("Serial Monitor", ImVec2(-1, -1), true, serial_monitor_flags);
    {
        std::lock_guard<std::mutex> lock(gui->serial_manager.mtx);
        if (textInputRecieved)
        {
            gui->serial_manager.send_serial(reinterpret_cast<unsigned char*>(buf));
            gui->PrintBuffer.push_back(buf);
        }
        for (size_t i = 0; i < gui->PrintBuffer.size(); i++){
            ImGui::Text(gui->PrintBuffer.get_vector()[i].c_str());
        }
    }
    if (auto_scroll) ImGui::SetScrollY(ImGui::GetScrollMaxY());
    ImGui::EndChild();
}

} // namespace bsp