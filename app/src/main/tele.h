if (ImGui::Button(Teleport ? "ON" : "OFF", btnSize))
{
    Teleport = true;
    TeleportTime = ImGui::GetTime(); 
}


if (Teleport && (ImGui::GetTime() - TeleportTime) >= 0.4f)
{
    Teleport = false;
}

bool Teleport = false;
