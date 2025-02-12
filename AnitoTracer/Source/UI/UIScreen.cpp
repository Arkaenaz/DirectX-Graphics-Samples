#include "pch.h"
#include "UIScreen.h"

#include "Texture.h"
#include "TextureManager.h"

UIScreen::UIScreen(String name) : isActive(true), name(name){}
UIScreen::~UIScreen() {}

void UIScreen::textCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}

bool UIScreen::buttonCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	return ImGui::Button(text.c_str());
}

void UIScreen::imageCentered(const wchar_t* path, float sizeMult)
{
	// Get Texture
	std::wstring filepath = L"Assets/Textures/Logo.jpg";
	TextureRef texture = TextureManager::LoadDDSFromFile(filepath, Graphics::kWhiteOpaque2D, true);
	
	// Calculate left padding
	ImVec2 imageSize = ImVec2(texture->GetWidth() * sizeMult, texture->GetHeight() * sizeMult);
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 padding = { (windowSize.x - imageSize.x) * 0.5f, ImGui::GetCursorPosY() };
	ImGui::SetCursorPos(padding);

	ImGui::Image((ImTextureID)texture->GetResource(), imageSize);
}

UIScreen::String UIScreen::getName()
{
	return this->name;
}

bool UIScreen::getActive()
{
	return this->isActive;
}

void UIScreen::setActive(bool active)
{
	this->isActive = active;
}
