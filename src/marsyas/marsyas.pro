include( $$quote( "$$BASEDIR/marsyasConfig.pri" ) )

message( Configuring Marsyas Lib build )

TEMPLATE = lib
CONFIG += staticlib
TARGET = marsyas

CONFIG(release, debug|release) {
  message(Building with release support.)
  DESTDIR = $$quote( "$$BASEDIR/lib/release/" )
}

CONFIG(debug, debug|release) {
  message(Building with debug support.)
  DESTDIR = $$quote( "$$BASEDIR/lib/debug/" )
}

######################################################################
# RtAudio Sources
######################################################################
marsyasAUDIOIO {
	SOURCES += \
		$$quote( "$$BASEDIR/otherlibs/RtAudio/RtAudio.cpp" )
		
	HEADERS += \
		$$quote( "$$BASEDIR/otherlibs/RtAudio/RtAudio.h" ) \
		$$quote( "$$BASEDIR/otherlibs/RtAudio/RtError.h" )
		
	win32{
		marsyasAUDIOIO_ASIO {
			SOURCES += 
				$$quote( "$$BASEDIR/otherlibs/asio/asio.cpp" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/asiodrivers.cpp" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/asiolist.cpp" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/iasiothiscallresolver.cpp" )
			HEADERS += \ 
				$$quote( "$$BASEDIR/otherlibs/asio/asio.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/asiodrivers.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/asiodrvr.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/asiolist.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/asiosys.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/ginclude.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/iasiodrv.h" ) \
				$$quote( "$$BASEDIR/otherlibs/asio/iasiothiscallresolver.h" )
		}
	}	
}

######################################################################
# RtMidi Sources
######################################################################
marsyasMIDIIO {
	SOURCES += \
		$$quote( "$$BASEDIR/otherlibs/RtMidi/RtMidi.cpp" )
		
	HEADERS += \
		$$quote( "$$BASEDIR/otherlibs/RtMidi/RtMidi.h" ) \
		$$quote( "$$BASEDIR/otherlibs/RtMidi/RtError.h" )
}

######################################################################
# Marsyas GUI source files
######################################################################
marsyasQT {
	SOURCES += \
		Qt/GainControlsGUI.cpp \
		Qt/MATLABeditorGUI.cpp \
		Qt/MarControlGUI.cpp \
		Qt/MarGUIManager.cpp \
		Qt/MarSystemControlsGUI.cpp \
		Qt/MarSystemNetworkGUI.cpp \
		Qt/MarSystemThread.cpp \
		Qt/SoundFileSource2ControlsGUI.cpp
		
	HEADERS += \
		Qt/GainControlsGUI.h \
		Qt/MATLABeditorGUI.h \
		Qt/MarControlGUI.h \
		Qt/MarGUIManager.h \
		Qt/MarSystemControlsGUI.h \
		Qt/MarSystemNetworkGUI.h \
		Qt/MarSystemThread.h \
		Qt/SoundFileSource2ControlsGUI.h
}

######################################################################
# Marsyas MATLAB engine input files
######################################################################
marsyasMATLABdebug {
	HEADERS += ./MATLAB/MATLABengine.h
	SOURCES += ./MATLAB/MATLABengine.cpp
}
marsyasMATLABrelease {
	HEADERS += ./MATLAB/MATLABengine.h
	SOURCES += ./MATLAB/MATLABengine.cpp
}

######################################################################
# libSVM
######################################################################
INCLUDEPATH += $$quote( "$$BASEDIR/otherlibs/libsvm" )
HEADERS += $$quote( "$$BASEDIR/otherlibs/libsvm/svm.h" )
SOURCES += $$quote( "$$BASEDIR/otherlibs/libsvm/svm.cpp" )

######################################################################
# Marsyas Core input files
######################################################################
HEADERS += \
	ADSR.h \
	AMDF.h \
	ANN_node.h \
	AbsMax.h \
	AbsSoundFileSink.h \
	AbsSoundFileSource.h \
	Accumulator.h \
	Annotator.h \
	AuFileSink.h \
	AuFileSource.h \
	AudioSink.h \
	AudioSource.h \
	AutoCorrelation.h \
	BaseAudioSink.h \
	BeatHistoFeatures.h \
	Cascade.h \
	Centroid.h \
	ClassOutputSink.h \
	ClipAudioRange.h \
	Collection.h \
	CollectionFileSource.h \
	Color.h \
	CommandLineOptions.h \
	Communicator.h \
	CompExp.h \
	Confidence.h \
	Conversions.h \
	Daub4.h \
	Delay.h \
	DownSampler.h \
	ERB.h \
	Envelope.h \
	Esitar.h \
	EvExpr.h \
	EvGetUpd.h \
	EvValUpd.h \
	ExCommon.h \
	ExNode.h \
	ExParser.h \
	ExScanner.h \
	ExSymTbl.h \
	ExVal.h \
	Expr.h \
	FM.h \
	Fanin.h \
	Fanout.h \
	FileName.h \
	Filter.h \
	Flux.h \
	FullWaveRectifier.h \
	GaussianClassifier.h \
	HalfWaveRectifier.h \
	HarmonicEnhancer.h \
	Heap.h \
	Histogram.h \
	InvSpectrum.h \
	KNNClassifier.h \
	Kurtosis.h \
	LPC.h \
	LPCC.h \
	LSP.h \
	Limiter.h \
	MFCC.h \
	MP3FileSource.h \
	MarControl.h \
	MarControlManager.h \
	MarControlValue.h \
	MarEvent.h \
	MarFileSink.h \
	MarSystem.h \
	MarSystemManager.h \
	MarSystemTemplateAdvanced.h \
	MarSystemTemplateBasic.h \
	MaxArgMax.h \
	Mean.h \
	MeddisHairCell.h \
	Memory.h \
	MemorySource.h \
	#Messager.h \
	MidiInput.h \
	MidiOutput.h \
	MinArgMin.h \
	Mono2Stereo.h \
	MrsLog.h \
	MslModel.h \
	Negative.h \
	#NetworkSocket.h \
	#NetworkTCPSink.h \
	#NetworkTCPSource.h \
	#NetworkUDPSink.h \
	#NetworkUDPSource.h \
	NoiseGate.h \
	NoiseSource.h \
	Norm.h \
	NormCut.h \
	NormMaxMin.h \
	Normalize.h \
	NumericLib.h \
	OggFileSource.h \
	OnePole.h \
	OverlapAdd.h \
	PCA.h \
	Parallel.h \
	PeakConvert.h \
	PeakResidual.h \
	PeUtilities.h \
	Peak2Rms.h \
	PeakPeriods2BPM.h \
	Peaker.h \
	PeakerAdaptive.h \
	PlotSink.h \
	Plucked.h \
	Power.h \
	PowerSpectrum.h \
	Product.h \
	PvConvert.h \
	PvConvolve.h \
	PvFold.h \
	PvOscBank.h \
	PvOverlapadd.h \
	PvUnconvert.h \
	RawFileSource.h \
	RealvecSink.h \
	RealvecSource.h \
	Reassign.h \
	Repeat.h \
	Rms.h \
	Rolloff.h \
	SCF.h \
	SFM.h \
	SMO.h \
	SOM.h \
	ScheduledEvent.h \
	Scheduler.h \
	SchedulerManager.h \
	SeneffEar.h \
	Series.h \
	#Server.h \
	ShiftInput.h \
	ShiftOutput.h \
	Shifter.h \
	Shredder.h \
	SilenceRemove.h \
	SineSource.h \
	Skewness.h \
	#Socket.h \
	#SocketException.h \
	SoundFileSink.h \
	SoundFileSource.h \
	Spectrum.h \
	StandardDeviation.h \
	StereoSpectrum.h \
	StereoSpectrumFeatures.h \
	Stk.h \
	Sum.h \
	Summary.h \
	Talk.h \
	Thread.h \
	TimeLine.h \
	TimeStretch.h \
	TimerFactory.h \
	TmControlValue.h \
	TmGetTime.h \
	TmSampleCount.h \
	TmTime.h \
	TmTimer.h \
	VScheduler.h \
	ViconFileSource.h \
	WavFileSink.h \
	WavFileSource.h \
	WaveletBands.h \
	WaveletPyramid.h \
	WaveletStep.h \
	WekaSink.h \
	WekaSource.h \
	Windowing.h \
	ZeroCrossings.h \
	ZeroRClassifier.h \
	basis.h \
	common.h \
	dsound.h \
	fft.h \
	lu.h \
	mididevices.h \
	realvec.h \
	vmblock.h \
	PeakSynthOsc.h \
	Cartesian2Polar.h \
	PeakSynthFFT.h \
	Polar2Cartesian.h \
	FlowCutSource.h \
	AbsSoundFileSource2.h \
	AudioSink2.h \
	AudioSource2.h \
	SoundFileSource2.h \
	WavFileSource2.h \
	BICchangeDetector.h \
	QGMMModel.h \
	PeakSynthOscBank.h \
	SpectralSNR.h \
	Vibrato.h \
	Panorama.h \
	Biquad.h \
	FlowThru.h \
	FanOutIn.h \
	MarSystemTemplateMedium.h \
	PeakFeatureSelect.h \
	SimilarityMatrix.h \
	Metric.h \
	HWPS.h \
	RBF.h \
	NormMatrix.h \
	WHaSp.h \
	PeakLabeler.h \
	PeakClusterSelect.h \
	peakView.h \
	PeakViewSink.h \
	PeakViewSource.h \
	OneRClassifier.h \
	WekaData.h \
	TranscriberExtract.h \
	Transcriber.h \
	PhiSEMSource.h \
	PhiSEMFilter.h \
	MarControlAccessor.h \
	SVMClassifier.h \
	Classifier.h \
	Spectrum2Chroma.h \
	Gain.h
# please leave Gain.h at the end like this; it makes scripts happy.
	 
SOURCES += \
	ADSR.cpp \
	AMDF.cpp \
	ANN_node.cpp \
	AbsMax.cpp \
	AbsSoundFileSink.cpp \
	AbsSoundFileSource.cpp \
	Accumulator.cpp \
	Annotator.cpp \
	AuFileSink.cpp \
	AuFileSource.cpp \
	AudioSink.cpp \
	AudioSource.cpp \
	AutoCorrelation.cpp \
	BaseAudioSink.cpp \
	BeatHistoFeatures.cpp \
	Cascade.cpp \
	Centroid.cpp \
	ClassOutputSink.cpp \
	ClipAudioRange.cpp \
	Collection.cpp \
	CollectionFileSource.cpp \
	Color.cpp \
	CommandLineOptions.cpp \
	Communicator.cpp \
	CompExp.cpp \
	Confidence.cpp \
	Conversions.cpp \
	Daub4.cpp \
	Delay.cpp \
	DownSampler.cpp \
	ERB.cpp \
	Envelope.cpp \
	Esitar.cpp \
	EvExpr.cpp \
	EvGetUpd.cpp \
	EvValUpd.cpp \
	ExCommon.cpp \
	ExNode.cpp \
	ExParser.cpp \
	ExScanner.cpp \
	ExSymTbl.cpp \
	ExVal.cpp \
	Expr.cpp \
	FM.cpp \
	Fanin.cpp \
	Fanout.cpp \
	FileName.cpp \
	Filter.cpp \
	Flux.cpp \
	FullWaveRectifier.cpp \
	GaussianClassifier.cpp \
	HalfWaveRectifier.cpp \
	HarmonicEnhancer.cpp \
	Histogram.cpp \
	InvSpectrum.cpp \
	KNNClassifier.cpp \
	Kurtosis.cpp \
	LPC.cpp \
	LPCC.cpp \
	LSP.cpp \
	Limiter.cpp \
	MFCC.cpp \
	MP3FileSource.cpp \
	MarControl.cpp \
	MarControlManager.cpp \
	MarControlValue.cpp \
	MarEvent.cpp \
	MarFileSink.cpp \
	MarSystem.cpp \
	MarSystemManager.cpp \
	MarSystemTemplateAdvanced.cpp \
	MarSystemTemplateBasic.cpp \
	MaxArgMax.cpp \
	Mean.cpp \
	MeddisHairCell.cpp \
	Memory.cpp \
	MemorySource.cpp \
	#Messager.cpp \
	MidiInput.cpp \
	MidiOutput.cpp \
	MinArgMin.cpp \
	Mono2Stereo.cpp \
	MrsLog.cpp \
	MslModel.cpp \
	Negative.cpp \
	#NetworkSocket.cpp \
	#NetworkTCPSink.cpp \
	#NetworkTCPSource.cpp \
	#NetworkUDPSink.cpp \
	#NetworkUDPSource.cpp \
	NoiseGate.cpp \
	NoiseSource.cpp \
	Norm.cpp \
	NormCut.cpp \
	NormMaxMin.cpp \
	Normalize.cpp \
	NumericLib.cpp \
	OggFileSource.cpp \
	OnePole.cpp \
	OverlapAdd.cpp \
	PCA.cpp \
	Parallel.cpp \
	PeakConvert.cpp \
	PeakResidual.cpp \
	PeUtilities.cpp \
	Peak2Rms.cpp \
	PeakPeriods2BPM.cpp \
	Peaker.cpp \
	PeakerAdaptive.cpp \
	PlotSink.cpp \
	Plucked.cpp \
	Power.cpp \
	PowerSpectrum.cpp \
	Product.cpp \
	PvConvert.cpp \
	PvConvolve.cpp \
	PvFold.cpp \
	PvOscBank.cpp \
	PvOverlapadd.cpp \
	PvUnconvert.cpp \
	RawFileSource.cpp \
	RealvecSink.cpp \
	RealvecSource.cpp \
	Reassign.cpp \
	Repeat.cpp \
	Rms.cpp \
	Rolloff.cpp \
	SCF.cpp \
	SFM.cpp \
	SMO.cpp \
	SOM.cpp \
	ScheduledEvent.cpp \
	Scheduler.cpp \
	SchedulerManager.cpp \
	SeneffEar.cpp \
	Series.cpp \
	#Server.cpp \
	ShiftInput.cpp \
	ShiftOutput.cpp \
	Shifter.cpp \
	Shredder.cpp \
	SilenceRemove.cpp \
	SineSource.cpp \
	Skewness.cpp \
	#Socket.cpp \
	SoundFileSink.cpp \
	SoundFileSource.cpp \
	Spectrum.cpp \
	StandardDeviation.cpp \
	StereoSpectrum.cpp \
	StereoSpectrumFeatures.cpp \
	Stk.cpp \
	Sum.cpp \
	Summary.cpp \
	Talk.cpp \
	Thread.cpp \
	TimeLine.cpp \
	TimeStretch.cpp \
	TimerFactory.cpp \
	TmControlValue.cpp \
	TmGetTime.cpp \
	TmSampleCount.cpp \
	TmTime.cpp \
	TmTimer.cpp \
	VScheduler.cpp \
	ViconFileSource.cpp \
	WavFileSink.cpp \
	WavFileSource.cpp \
	WaveletBands.cpp \
	WaveletPyramid.cpp \
	WaveletStep.cpp \
	WekaSink.cpp \
	WekaSource.cpp \
	Windowing.cpp \
	ZeroCrossings.cpp \
	ZeroRClassifier.cpp \
	basis.cpp \
	fft.cpp \
	lu.cpp \
	realvec.cpp \
	vmblock.cpp \
	PeakSynthOsc.cpp \
	Cartesian2Polar.cpp \
	PeakSynthFFT.cpp \
	Polar2Cartesian.cpp \
	FlowCutSource.cpp \
	AbsSoundFileSource2.cpp \
	AudioSink2.cpp \
	AudioSource2.cpp \
	SoundFileSource2.cpp \
	WavFileSource2.cpp \
	BICchangeDetector.cpp \
	QGMMModel.cpp \
	PeakSynthOscBank.cpp \
	SpectralSNR.cpp \
	Vibrato.cpp \
	Panorama.cpp \
	Biquad.cpp \
	FlowThru.cpp \
	MarSystemTemplateMedium.cpp \
	PeakFeatureSelect.cpp \
	SimilarityMatrix.cpp \
	Metric.cpp \
	HWPS.cpp \
	RBF.cpp \
	NormMatrix.cpp \
	WHaSp.cpp \
	FanOutIn.cpp \
	PeakLabeler.cpp \
	PeakClusterSelect.cpp \
	peakView.cpp \
	PeakViewSink.cpp \
	PeakViewSource.cpp \
	OneRClassifier.cpp \
	WekaData.cpp \
	TranscriberExtract.cpp \
	Transcriber.cpp \
	PhiSEMSource.cpp \
	PhiSEMFilter.cpp \
	SVMClassifier.cpp \
	Classifier.cpp \
	Spectrum2Chroma.cpp \
	Gain.cpp
# please leave Gain.cpp at the end like this; it makes scripts happy.

