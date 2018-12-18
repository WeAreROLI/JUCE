#include "RulersView.h"
#include "ARASampleProjectAudioProcessorEditor.h"

//==============================================================================
RulersView::RulersView (ARASampleProjectAudioProcessorEditor& owner)
    : owner (owner),
      document (nullptr),
      musicalContext (nullptr)
{
    setColour (borderColourId, Colours::darkgrey);
    setColour (musicalRulerBackgroundColourId, (Colours::green).withAlpha(0.2f));
    setColour (timeRulerBackgroundColourId, (Colours::blue).withAlpha(0.2f));
    setColour (chordsRulerBackgroundColourId, Colours::transparentBlack);
    setColour (musicalGridColourId, Colours::slategrey);
    setColour (timeGridColourId, Colours::slateblue);
    setColour (chordsColourId, Colours::slategrey);

    if (owner.isARAEditorView())
    {
        document = owner.getARADocumentController()->getDocument<ARADocument>();
        document->addListener (this);
        findMusicalContext();
    }
}

RulersView::~RulersView()
{
    detachFromMusicalContext();
    detachFromDocument();
}

void RulersView::detachFromDocument()
{
    if (document == nullptr)
        return;

    document->removeListener (this);

    document = nullptr;
}

void RulersView::detachFromMusicalContext()
{
    if (musicalContext == nullptr)
        return;

    musicalContext->removeListener (this);

    musicalContext = nullptr;
}

void RulersView::findMusicalContext()
{
    if (! owner.isARAEditorView())
        return;

    // evaluate selection
    ARAMusicalContext* newMusicalContext = nullptr;
    auto viewSelection = owner.getARAEditorView()->getViewSelection();
    if (! viewSelection.getRegionSequences().empty())
        newMusicalContext = viewSelection.getRegionSequences().front()->getMusicalContext<ARAMusicalContext>();
    else if (! viewSelection.getPlaybackRegions().empty())
        newMusicalContext = viewSelection.getPlaybackRegions().front()->getRegionSequence()->getMusicalContext<ARAMusicalContext>();

    // if no context used yet and selection does not yield a new one, use the first musical context in the docment
    if (musicalContext == nullptr && newMusicalContext == nullptr &&
        ! owner.getARADocumentController()->getDocument()->getMusicalContexts().empty())
    {
        newMusicalContext = owner.getARADocumentController()->getDocument()->getMusicalContexts<ARAMusicalContext>().front();
    }

    if (newMusicalContext != musicalContext)
    {
        detachFromMusicalContext();

        musicalContext = newMusicalContext;
        musicalContext->addListener (this);

        repaint();
    }
}

//==============================================================================
void RulersView::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds();

    if (musicalContext == nullptr)
    {
        g.setColour (Colours::darkgrey);
        g.drawRect (bounds, 3);

        g.setColour (Colours::white);
        g.setFont (Font (12.0f));
        g.drawText ("No musical context found in ARA document!", bounds, Justification::centred);
        
        return;
    }

    // we'll draw three rulers: seconds, beats, and chords
    constexpr int lightLineWidth = 1;
    constexpr int heavyLineWidth = 3;
    const int chordRulerY = 0;
    const int chordRulerHeight = bounds.getHeight() / 3;
    const int beatsRulerY = chordRulerY + chordRulerHeight;
    const int beatsRulerHeight = (bounds.getHeight() - chordRulerHeight) / 2;
    const int secondsRulerY = beatsRulerY + beatsRulerHeight;
    const int secondsRulerHeight = bounds.getHeight() - chordRulerHeight - beatsRulerHeight;

    // we should only be doing this on the visible time range
    double startTime, endTime;
//  TODO JUCE_ARA getVisibleTimeRange() does not work properly - for now, we're drawing the entire timeline...
//  owner.getVisibleTimeRange (startTime, endTime);
    owner.getTimeRange (startTime, endTime);

    // seconds ruler: one tick for each second
    {
        g.setColour (findColour (ColourIds::timeRulerBackgroundColourId));
        g.fillRect (bounds.getX(), secondsRulerY, bounds.getWidth(), secondsRulerHeight);

        RectangleList<int> rects;
        const double lastSecond = floor (endTime);
        for (double nextSecond = ceil (startTime); nextSecond <= lastSecond; nextSecond += 1.0)
        {
            int lineWidth = (nextSecond == 0.0) ? heavyLineWidth : lightLineWidth;
            rects.addWithoutMerging (Rectangle<int> (owner.getPlaybackRegionsViewsXForTime(nextSecond) - lineWidth / 2, secondsRulerY, lineWidth, secondsRulerHeight));
        }
        g.setColour (findColour (ColourIds::timeGridColourId));
        g.fillRectList (rects);
    }

    // beat ruler: evaluates tempo and bar signatures to draw a line for each beat
    {
        // use our musical context to read tempo and bar signature data using content readers
        ARA::PlugIn::HostContentReader<ARA::kARAContentTypeTempoEntries> tempoReader (musicalContext);
        ARA::PlugIn::HostContentReader<ARA::kARAContentTypeBarSignatures> barSignatureReader (musicalContext);

        // we must have at least two tempo entries and a bar signature in order to have a proper timing information
        const int tempoEntryCount = tempoReader.getEventCount();
        const int barSigEventCount = barSignatureReader.getEventCount();
        jassert (tempoEntryCount >= 2 && barSigEventCount >= 1);

        g.setColour (findColour (ColourIds::musicalRulerBackgroundColourId));
        g.fillRect (bounds.getX(), beatsRulerY, bounds.getWidth(), beatsRulerHeight);
        RectangleList <int> beatsRects;

        // find the first tempo entry for our starting time
        int ixT = 0;
        for (; ixT < tempoEntryCount - 2 && tempoReader.getDataForEvent (ixT + 1)->timePosition < startTime; ++ixT);

        // use a lambda to update our tempo state while reading the host tempo map
        double tempoBPM (120);
        double secondsToBeats (0), pixelsPerBeat (0);
        double beatEnd (0);
        auto updateTempoState = [&, this] (bool advance)
        {
            if (advance)
                ++ixT;

            double deltaT = (tempoReader.getDataForEvent (ixT + 1)->timePosition - tempoReader.getDataForEvent (ixT)->timePosition);
            double deltaQ = (tempoReader.getDataForEvent (ixT + 1)->quarterPosition - tempoReader.getDataForEvent (ixT)->quarterPosition);
            tempoBPM = 60 * deltaQ / deltaT;
            secondsToBeats = (tempoBPM / 60);
            pixelsPerBeat = owner.getPixelsPerSecond() / secondsToBeats;
            beatEnd = secondsToBeats * endTime;
        };

        // update our tempo state using the first two tempo entries
        updateTempoState (false);

        // convert the starting time to beats
        double beatStart = secondsToBeats * startTime;

        // get the bar signature entry just before beat start (or the last bar signature in the reader)
        int ixB = 0;
        for (; ixB < barSigEventCount - 1 && barSignatureReader.getDataForEvent (ixB + 1)->position < beatStart; ++ixB);
        int barSigNumerator = barSignatureReader.getDataForEvent (ixB)->numerator;

        // find the next whole beat and see if it's in our range
        int nextWholeBeat = roundToInt (ceil (beatStart));
        if (nextWholeBeat < beatEnd)
        {
            // read the tempo map to find the starting beat position in pixels
            double beatPixelPosX = 0;
//            double beatsTillWholeBeat = nextWholeBeat - beatStart;
            for (; ixT < tempoEntryCount - 2 && tempoReader.getDataForEvent (ixT + 1)->quarterPosition < nextWholeBeat;)
            {
                beatPixelPosX += pixelsPerBeat * (tempoReader.getDataForEvent (ixT + 1)->quarterPosition - tempoReader.getDataForEvent (ixT)->quarterPosition);
                updateTempoState (true);
            }

            if (tempoReader.getDataForEvent (ixT)->quarterPosition < nextWholeBeat)
                beatPixelPosX += pixelsPerBeat * (nextWholeBeat - tempoReader.getDataForEvent (ixT)->quarterPosition);

            // use a lambda to draw beat markers
            auto drawBeatRects = [&] (int beatsToDraw)
            {
                // for each beat, advance beat pixel by the current pixelsPerBeat value
                for (int b = 0; b < beatsToDraw; b++)
                {
                    int curBeat = nextWholeBeat + b;
                    int tickWidth = ((curBeat % barSigNumerator) == 0) ? heavyLineWidth : lightLineWidth;
                    beatsRects.addWithoutMerging (Rectangle<int> (roundToInt(beatPixelPosX), beatsRulerY, tickWidth, beatsRulerHeight));
                    beatPixelPosX += pixelsPerBeat;
                }
            };

            // read tempo entries from the host tempo map until we run out of entries or reach endTime
            while (ixT < tempoEntryCount - 2 && tempoReader.getDataForEvent (ixT + 1)->timePosition < endTime)
            {
                // draw rects for each whole beat from nextWholeBeat to the next tempo entry
                // keep offsetting pixelStartBeats so we know where to draw the next one

                // draw a beat rect for each beat that's passed since we
                // drew a beat marker and advance to the next whole beat
                int beatsToNextTempoEntry = (int) (tempoReader.getDataForEvent (ixT)->quarterPosition - nextWholeBeat);
                drawBeatRects (beatsToNextTempoEntry);
                nextWholeBeat += beatsToNextTempoEntry;

                // find the new tempo
                updateTempoState (true);

                // advance bar signature numerator if our beat position passes the most recent entry
                if (ixB < barSigEventCount - 1 && barSignatureReader.getDataForEvent(ixB)->position < nextWholeBeat)
                    barSigNumerator = barSignatureReader.getDataForEvent (++ixB)->numerator;
            }

            // draw the remaining rects until beat end
            int remainingBeats = roundToInt (ceil (beatEnd) - nextWholeBeat);
            drawBeatRects (remainingBeats);
        }

        g.setColour (findColour (ColourIds::musicalGridColourId));
        g.fillRectList (beatsRects);
    }

    // TODO JUCE_ARA chord ruler
    {
        g.setColour (findColour (ColourIds::chordsRulerBackgroundColourId));
        g.fillRect (bounds.getX(), chordRulerY, bounds.getWidth(), chordRulerHeight);

        //g.setColour (findColour (ColourIds::chordsColourId));
    }

    // borders
    {
        g.setColour (findColour (ColourIds::borderColourId));
        g.drawLine (bounds.getX(), beatsRulerY, bounds.getWidth(), beatsRulerY);
        g.drawLine (bounds.getX(), secondsRulerY, bounds.getWidth(), secondsRulerY);
        g.drawRect (bounds);
    }
}

//==============================================================================

void RulersView::onNewSelection (const ARA::PlugIn::ViewSelection& currentSelection)
{
    findMusicalContext();
}

void RulersView::didEndEditing (ARADocument* /*doc*/)
{
    if (musicalContext == nullptr)
        findMusicalContext();
}

void RulersView::willRemoveMusicalContextFromDocument (ARADocument* doc, ARAMusicalContext* context)
{
    jassert (document == doc);

    if (musicalContext == context)
        detachFromMusicalContext();     // will restore in didEndEditing()
}

void RulersView::didReorderMusicalContextsInDocument (ARADocument* doc)
{
    jassert (document == doc);

    if (musicalContext != document->getMusicalContexts().front())
        detachFromMusicalContext();     // will restore in didEndEditing()
}

void RulersView::willDestroyDocument (ARADocument* doc)
{
    jassert (document == doc);

    detachFromDocument();
}

void RulersView::doUpdateMusicalContextContent (ARAMusicalContext* context, ARAContentUpdateScopes /*scopeFlags*/)
{
    jassert (musicalContext == context);

    repaint();
}
