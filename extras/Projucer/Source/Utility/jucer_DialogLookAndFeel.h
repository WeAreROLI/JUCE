/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef PROJUCER_LOOKANDFEEL_H_INCLUDED
#define PROJUCER_LOOKANDFEEL_H_INCLUDED


class ProjucerDialogLookAndFeel  : public LookAndFeel_V3
{
public:
    //==============================================================================
    const float labelFontSize  = 12.0f;
    const float buttonFontSize = 15.0f;

    //==============================================================================
    void drawToggleButton (Graphics& g, ToggleButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/) override
    {
        g.setColour (Colours::white);
        g.fillEllipse (4.0f, 4.0f, 13.0f, 13.0f);

        if (button.getToggleState())
        {
            g.setColour (Colours::black);
            g.fillEllipse (6.0f, 6.0f, 9.0f, 9.0f);
        }

        g.setColour (button.findColour (ToggleButton::textColourId));
        g.setFont (getContaxProFont().withHeight (labelFontSize));

        g.drawFittedText (button.getButtonText(), 24, 1,
                          button.getWidth() - 24, button.getHeight(),
                          Justification::centredLeft, 10);
    }

    void drawButtonBackground (Graphics& g, Button& button, const Colour& /*backgroundColour*/,
                               bool isMouseOverButton, bool isButtonDown) override
    {
        auto buttonRect = button.getLocalBounds().toFloat();

        if (button.getProperties()["isSecondaryButton"])
            drawSecondaryButtonBackground (g, buttonRect, isMouseOverButton, isButtonDown);
        else
            drawPrimaryButtonBackground (g, buttonRect, isMouseOverButton, isButtonDown);
    }

    void drawButtonText (Graphics& g, TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        Font font (getTextButtonFont (button, button.getHeight()));
        g.setFont (font);

        if (button.getProperties()["isSecondaryButton"])
            g.setColour (getBrightButtonColour (isMouseOverButton, isButtonDown));
        else
            g.setColour (getBackgroundColour());

        g.drawFittedText (button.getButtonText(), 0, 1,
                          button.getWidth(),
                          button.getHeight(),
                          Justification::centred, 2);
    }

    //==============================================================================
    Font getTextButtonFont (TextButton&, int /*buttonHeight*/) override
    {
        return getContaxProFont().withHeight (buttonFontSize);
    }

    Font getLabelFont (Label&) override
    {
        return getContaxProFont().withHeight (labelFontSize);
    }

    static const Font& getContaxProFont()
    {
        static Font font (Typeface::createSystemTypefaceFor (BinaryData::ContaxPro55Rm_otf, BinaryData::ContaxPro55Rm_otfSize));
        return font;
    }

    //==============================================================================
    int getAlertWindowButtonHeight() override   { return 40; }

    Font getAlertWindowTitleFont() override     { return getContaxProFont().withHeight (18); }
    Font getAlertWindowMessageFont() override   { return getContaxProFont().withHeight (12); }
    Font getAlertWindowFont() override          { return getContaxProFont().withHeight (12); }

    //==============================================================================
    static Colour getBackgroundColour()         { return Colour (0xff4d4d4d); }
    static Colour getBrightButtonColour()       { return Colour (0xffe6e6e6); }
    static Colour getErrorTextColour()          { return Colour (0xfff390a2); }

    static Colour getBrightButtonColour (bool isMouseOverButton, bool isButtonDown)
    {
        if (isButtonDown)       return getBrightButtonColour().withAlpha (0.7f);
        if (isMouseOverButton)  return getBrightButtonColour().withAlpha (0.85f);
        return getBrightButtonColour();
    }

private:
    //==============================================================================
    void drawPrimaryButtonBackground (Graphics& g,
                                      Rectangle<float> buttonRect,
                                      bool isMouseOverButton,
                                      bool isButtonDown)
    {
        g.setColour (getBrightButtonColour (isMouseOverButton, isButtonDown));
        g.fillRoundedRectangle (buttonRect, 5.0f);
    }

    void drawSecondaryButtonBackground (Graphics& g,
                                        Rectangle<float> buttonRect,
                                        bool isMouseOverButton,
                                        bool isButtonDown)
    {
        g.setColour (getBrightButtonColour (isMouseOverButton, isButtonDown));
        g.drawRoundedRectangle (buttonRect.reduced (1.0f), 5.0f, 2.0f);
    }
};



#endif  // PROJUCER_LOOKANDFEEL_H_INCLUDED
