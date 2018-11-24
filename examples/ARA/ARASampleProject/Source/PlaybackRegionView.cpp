#include "PlaybackRegionView.h"
#include "ARASampleProjectDocumentController.h"
#include "ARASampleProjectAudioProcessorEditor.h"

PlaybackRegionView::PlaybackRegionView (ARASampleProjectAudioProcessorEditor* editor, ARAPlaybackRegion* region)
: editorComponent (editor),
  playbackRegion (region),
  isSelected (false),
  audioThumbCache (1),
  audioThumb (128, audioFormatManger, audioThumbCache)
{
    audioThumb.addChangeListener (this);

    static_cast<ARADocument*> (playbackRegion->getRegionSequence()->getDocument())->addListener (this);
    playbackRegion->addListener (this);
    recreatePlaybackRegionReader();
}

PlaybackRegionView::~PlaybackRegionView()
{
    static_cast<ARADocument*> (playbackRegion->getRegionSequence()->getDocument())->removeListener (this);
    playbackRegion->removeListener(this);
    audioThumb.clear();
    audioThumb.removeChangeListener (this);
}

void PlaybackRegionView::paint (Graphics& g)
{
    Colour regionColour;
    // TODO JUCE_ARA Studio One uses black as the default color, which looks bad...
    const ARA::ARAColor* colour = playbackRegion->getColor();
    if (! colour)
        colour = playbackRegion->getRegionSequence()->getColor();
    if (colour != nullptr)
    {
        regionColour = Colour::fromFloatRGBA (colour->r, colour->g, colour->b, 1.0f);
        g.fillAll (regionColour);
    }

    g.setColour (isSelected ? juce::Colours::yellow : juce::Colours::black);
    g.drawRect (getLocalBounds());

    if (getLengthInSeconds() != 0.0)
    {
        g.setColour (regionColour.contrasting (0.7f));
        audioThumb.drawChannels (g, getLocalBounds(), 0.0, getLengthInSeconds(), 1.0);
    }
}

void PlaybackRegionView::changeListenerCallback (juce::ChangeBroadcaster* /*broadcaster*/)
{
    repaint();
}

double PlaybackRegionView::getStartInSeconds() const
{
    return playbackRegion->getStartInPlaybackTime();
}

double PlaybackRegionView::getLengthInSeconds() const
{
    return playbackRegion->getDurationInPlaybackTime();
}

double PlaybackRegionView::getEndInSeconds() const
{
    return playbackRegion->getEndInPlaybackTime();
}

void PlaybackRegionView::willUpdatePlaybackRegionProperties (ARAPlaybackRegion* playbackRegion, ARAPlaybackRegion::PropertiesPtr newProperties)
{
    if ((playbackRegion->getStartInAudioModificationTime () != newProperties->startInModificationTime) ||
        (playbackRegion->getDurationInAudioModificationTime () != newProperties->durationInModificationTime) ||
        (playbackRegion->getStartInPlaybackTime () != newProperties->startInPlaybackTime) ||
        (playbackRegion->getDurationInPlaybackTime () != newProperties->durationInPlaybackTime))
    {
        editorComponent->setDirty ();
    }
}

void PlaybackRegionView::setIsSelected (bool value)
{
    bool needsRepaint = (value != isSelected);
    isSelected = value;
    if (needsRepaint)
        repaint();
}

void PlaybackRegionView::recreatePlaybackRegionReader()
{
    audioThumbCache.clear();

    // create a non-realtime playback region reader for our audio thumb
    auto documentController = static_cast<ARASampleProjectDocumentController*> (editorComponent->getARADocumentController());
    playbackRegionReader = documentController->createPlaybackRegionReader ({ playbackRegion }, true);
    
    // TODO JUCE_ARA
    // See juce_AudioThumbnail.cpp line 122
    if (playbackRegionReader->lengthInSamples <= 0)
    {
        delete playbackRegionReader;
        playbackRegionReader = nullptr;
    }
    else
    {
        audioThumb.setReader (playbackRegionReader, reinterpret_cast<intptr_t> (playbackRegion));   // TODO JUCE_ARA better hash?
    }
}

// TODO JUCE_ARA what if this is called after ARASampleProjectAudioProcessorEditor::doEndEditing?
void PlaybackRegionView::doEndEditing (ARADocument* /*document*/)
{
    if ((playbackRegionReader ==  nullptr) || ! playbackRegionReader->isValid())
    {
        recreatePlaybackRegionReader();
        editorComponent->setDirty(); 
    }
}
