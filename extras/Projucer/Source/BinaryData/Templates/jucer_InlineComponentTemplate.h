//==============================================================================
class %%component_class%%    : public Component
{
public:
    %%component_class%%()
    {
        // In your constructor, you should add any child components, and
        // initialize any special settings that your component needs.

    }

    ~%%component_class%%()
    {
    }

    void paint (Graphics& g) override
    {
        // You should replace everything in this method with your own drawing code..

        g.fillAll (getLookAndFeel().findColor (ResizableWindow::backgroundColorId));   // clear the background

        g.setColor (Colors::gray);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColor (Colors::white);
        g.setFont (14.0f);
        g.drawText ("%%component_class%%", getLocalBounds(),
                    Justification::centered, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%component_class%%)
};
