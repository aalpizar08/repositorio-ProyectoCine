#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Widgets {
private:
	sf::RenderWindow& window;
	sf::Font font;
	sf::Vector2f mouse;
	bool clicked;
	bool hadClick;
	std::string armedButton;
	bool isArmedButtonPressed;
	int focusedField;
public:
	Widgets(sf::RenderWindow& window);
	bool loadFont(const std::string& path);
	void beginFrame(bool isClicked);
	void endFrame();
	int getFocusedField() const;
	void setFocusedField(int field);
	bool isHovered(const sf::FloatRect& area) const;
	bool isPressed(const sf::FloatRect& area);
	bool drawButton(const std::string& label, const sf::FloatRect& area, bool isEnabled = true);
	bool drawConfirmButton(const std::string& key, const std::string& label, const sf::FloatRect& area, bool isEnabled = true);
	bool drawListRow(const sf::FloatRect& area, const std::string& title, const std::string& detail, const std::string& status, sf::Color statusColor, bool isEnabled = true);
	void drawTextField(int field, const std::string& label, const std::string& value, const sf::FloatRect& area);
	int drawSelector(const std::string& label, const std::string& value, const sf::FloatRect& area);
	void drawPanel(const sf::FloatRect& area, sf::Color color);
	float drawText(const std::string& value, float x, float y, unsigned int size, sf::Color color, bool isBold = false, float maxWidth = 0);
	void drawCenteredText(const std::string& value, float centerX, float centerY, unsigned int size, sf::Color color, bool isBold = false);
	float measureText(const std::string& value, unsigned int size, bool isBold = false);
	std::string fitText(const std::string& value, unsigned int size, float maxWidth, bool isBold = false);
	static std::string toUtf8(const sf::String& value);
	static sf::String fromUtf8(const std::string& value);
private:
	sf::Text createText(const std::string& value, unsigned int size, sf::Color color, bool isBold);
	std::vector<std::string> wrapText(const std::string& value, unsigned int size, bool isBold, float maxWidth);
	void wrapParagraph(const std::string& paragraph, unsigned int size, bool isBold, float maxWidth, std::vector<std::string>& lines);
	std::string fitTextEnd(const std::string& value, unsigned int size, float maxWidth);
};
