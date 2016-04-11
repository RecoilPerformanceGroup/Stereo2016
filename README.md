# Stereo2016

of.0.9.*

Addons:
- ofxOSC (core)
- ofxGui (core)
- ofxSmlSettings (core)
- ofxSvg (core)
- [ofxDatGui](https://github.com/mrbichel/ofxDatGui)
- [ofxEasing](https://github.com/arturoc/ofxEasing)
- [ofxVoro](https://github.com/mrbichel/ofxVoro)
- [ofxJSON](https://github.com/jefftimesten/ofxJSON)

CGAL:
- clone [ofxCGAL](https://github.com/yusuketomoto/ofxCGAL.git), but don't leave it in addons.make - we don't want to link against it's included cgal, use the one from instead:
- brew install cgal --with-eigen3

BOOST:
- make CoreOF.xconfig point to brew's boost in /usr/local/

Leap Motion OSC controller to input 3D position:
(https://github.com/mrbichel/LeapOSC)
