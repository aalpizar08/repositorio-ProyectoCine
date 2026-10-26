#include "widgets.h"
#include <sstream>
#include "palette.h"

namespace {
	const unsigned int buttonTextSize = 20;
	const unsigned int labelTextSize = 16;
	const unsigned int fieldTextSize = 20;
	const float fieldPadding = 14;
	const float selectorButtonWidth = 44;
	const std::string ellipsis = "...";
}

Widgets::Widgets(sf::RenderWindow& window)
	: window(window), clicked(false), hadClick(false), isArmedButtonPressed(false), focusedField(-1) {
}
bool Widgets::loadFont(const std::string& path) {
	return font.loadFromFile(path);
}
void Widgets::beginFrame(bool isClicked) {
	clicked = isClicked;
	hadClick = isClicked;
	isArmedButtonPressed = false;
	mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}
void Widgets::endFrame() {
	if (hadClick && !isArmedButtonPressed) {
		armedButton.clear();
	}
}
int Widgets::getFocusedField() const {
	return focusedField;
}
void Widgets::setFocusedField(int field) {
	focusedField = field;
}
bool Widgets::isHovered(const sf::FloatRect& area) const {
	return area.contains(mouse);
}
bool Widgets::isPressed(const sf::FloatRect& area) {
	if (!clicked || !isHovered(area)) {
		return false;
	}
	clicked = false;
	return true;
}
bool Widgets::drawButton(const std::string& label, const sf::FloatRect& area, bool isEnabled) {
	sf::Color color = Palette::disabled;
	if (isEnabled) {
		color = isHovered(area) ? Palette::accentHover : Palette::accent;
	}
	drawPanel(area, color);
	drawCenteredText(label, area.left + area.width / 2, area.top + area.height / 2, buttonTextSize, isEnabled ? Palette::text : Palette::muted, true);
	return isEnabled && isPressed(area);
}
bool Widgets::drawConfirmButton(const std::string& key, const std::string& label, const sf::FloatRect& area, bool isEnabled) {
	bool isWaiting = armedButton == key;
	if (!drawButton(isWaiting ? "¿Confirmar?" : label, area, isEnabled)) {
		return false;
	}
	isArmedButtonPressed = true;
	if (isWaiting) {
		armedButton.clear();
		return true;
	}
	armedButton = key;
	return false;
}
bool Widgets::drawListRow(const sf::FloatRect& area, const std::string& title, const std::string& detail, const std::string& status, sf::Color statusColor, bool isEnabled) {
	if (isEnabled && isHovered(area)) {
		drawPanel(area, Palette::rowHover);
	}
	float textWidth = area.width - 170;
	drawText(fitText(title, 17, textWidth, true), area.left + 16, area.top + 8, 17, isEnabled ? Palette::text : Palette::muted, true);
	drawText(fitText(detail, 15, textWidth), area.left + 16, area.top + 33, 15, Palette::muted);
	drawCenteredText(status, area.left + area.width - 75, area.top + area.height / 2, 16, statusColor, true);
	return isEnabled && isPressed(area);
}
void Widgets::drawTextField(int field, const std::string& label, const std::string& value, const sf::FloatRect& area) {
	bool isFocused = focusedField == field;
	drawText(label, area.left, area.top - 22, labelTextSize, Palette::muted);
	sf::RectangleShape box(sf::Vector2f(area.width, area.height));
	box.setPosition(area.left, area.top);
	box.setFillColor(Palette::field);
	box.setOutlineThickness(isFocused ? 2.f : 1.f);
	box.setOutlineColor(isFocused ? Palette::gold : Palette::disabled);
	window.draw(box);
	std::string shown = fitTextEnd(value + (isFocused ? "|" : ""), fieldTextSize, area.width - 2 * fieldPadding);
	drawText(shown, area.left + fieldPadding, area.top + (area.height - fieldTextSize) / 2 - 3, fieldTextSize, Palette::text);
	if (isPressed(area)) {
		focusedField = field;
	}
}
int Widgets::drawSelector(const std::string& label, const std::string& value, const sf::FloatRect& area) {
	drawText(label, area.left, area.top - 22, labelTextSize, Palette::muted);
	drawPanel(area, Palette::field);
	sf::FloatRect previous(area.left, area.top, selectorButtonWidth, area.height);
	sf::FloatRect next(area.left + area.width - selectorButtonWidth, area.top, selectorButtonWidth, area.height);
	float valueWidth = area.width - 2 * selectorButtonWidth - 2 * fieldPadding;
	drawCenteredText(fitText(value, fieldTextSize, valueWidth), area.left + area.width / 2, area.top + area.height / 2, fieldTextSize, Palette::text);
	if (drawButton("<", previous)) {
		return -1;
	}
	if (drawButton(">", next)) {
		return 1;
	}
	return 0;
}
void Widgets::drawPanel(const sf::FloatRect& area, sf::Color color) {
	sf::RectangleShape shape(sf::Vector2f(area.width, area.height));
	shape.setPosition(area.left, area.top);
	shape.setFillColor(color);
	window.draw(shape);
}
float Widgets::drawText(const std::string& value, float x, float y, unsigned int size, sf::Color color, bool isBold, float maxWidth) {
	sf::Text shape = createText("", size, color, isBold);
	float lineHeight = font.getLineSpacing(size);
	std::vector<std::string> lines = wrapText(value, size, isBold, maxWidth);
	for (std::size_t index = 0; index < lines.size(); index++) {
		shape.setString(fromUtf8(lines[index]));
		shape.setPosition(x, y + index * lineHeight);
		window.draw(shape);
	}
	return y + lines.size() * lineHeight;
}
void Widgets::drawCenteredText(const std::string& value, float centerX, float centerY, unsigned int size, sf::Color color, bool isBold) {
	sf::Text shape = createText(value, size, color, isBold);
	sf::FloatRect bounds = shape.getLocalBounds();
	shape.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
	shape.setPosition(centerX, centerY);
	window.draw(shape);
}
float Widgets::measureText(const std::string& value, unsigned int size, bool isBold) {
	return createText(value, size, Palette::text, isBold).getLocalBounds().width;
}
std::string Widgets::fitText(const std::string& value, unsigned int size, float maxWidth, bool isBold) {
	if (measureText(value, size, isBold) <= maxWidth) {
		return value;
	}
	sf::String shortened = fromUtf8(value);
	while (!shortened.isEmpty() && measureText(toUtf8(shortened) + ellipsis, size, isBold) > maxWidth) {
		shortened.erase(shortened.getSize() - 1);
	}
	return toUtf8(shortened) + ellipsis;
}
std::string Widgets::toUtf8(const sf::String& value) {
	std::basic_string<sf::Uint8> bytes = value.toUtf8();
	return std::string(bytes.begin(), bytes.end());
}
sf::String Widgets::fromUtf8(const std::string& value) {
	return sf::String::fromUtf8(value.begin(), value.end());
}
sf::Text Widgets::createText(const std::string& value, unsigned int size, sf::Color color, bool isBold) {
	sf::Text shape(fromUtf8(value), font, size);
	shape.setFillColor(color);
	shape.setStyle(isBold ? sf::Text::Bold : sf::Text::Regular);
	return shape;
}
std::vector<std::string> Widgets::wrapText(const std::string& value, unsigned int size, bool isBold, float maxWidth) {
	std::vector<std::string> lines;
	std::istringstream paragraphs(value);
	std::string paragraph;
	while (std::getline(paragraphs, paragraph)) {
		if (maxWidth <= 0) {
			lines.push_back(paragraph);
			continue;
		}
		wrapParagraph(paragraph, size, isBold, maxWidth, lines);
	}
	return lines;
}
void Widgets::wrapParagraph(const std::string& paragraph, unsigned int size, bool isBold, float maxWidth, std::vector<std::string>& lines) {
	std::istringstream words(paragraph);
	std::string word;
	std::string current;
	while (words >> word) {
		std::string attempt = current.empty() ? word : current + " " + word;
		if (measureText(attempt, size, isBold) > maxWidth && !current.empty()) {
			lines.push_back(current);
			current = word;
			continue;
		}
		current = attempt;
	}
	lines.push_back(current);
}
std::string Widgets::fitTextEnd(const std::string& value, unsigned int size, float maxWidth) {
	sf::String shortened = fromUtf8(value);
	while (!shortened.isEmpty() && measureText(toUtf8(shortened), size) > maxWidth) {
		shortened.erase(0);
	}
	return toUtf8(shortened);
}
