fbx-conv
========

Convert .fbx to .c3b or .c3t used by cocos2d-x. Thanks for libgdx fbx-conv. We used their code and do some change.

Precompiled Binaries
====================
You can download the precompiled binaries from https://github.com/cocos2d/cocos2d-x-3rd-party-libs-bin/.

Building
========
You'll need an installation of the FBX SDK 2014. Once installed, set the
FBX_SDK_ROOT to the directory where you installed the FBX SDK. Then run the project which you want at "build/xxxx/"
. These we generate Visual Studio and XCode project.

USAGE:
========
Open a command line and enter into the fbx-conv directory, and then input the full path fbx-conv file (you can drag fbx-conv to command line directly) and then enter the parameters -a (-a means export both of c3t and c3b file, if parameter is empty we export c3b file by default), and then enter the file name which you want to convert.

Command line: cd [path of fbx-conv] xx/xx/fbx-conv [options]

For example: 1. cd /Users/xx/cocos2d-x-3rd-party-libs-bin/fbx-conv/mac 2. /Users/xx/cocos2d-x-3rd-party-libs-bin/fbx-conv/mac/fbx-conv -a boss.FBX

You can use the -? option to see more help information.

Note:
========
1. Model need to have a material that contain one texture in the diffuse-channel at least.

2. About animation, we just support skeletal animation.

3. Just one skeletal animation object and no support multiple skeleton.

4. You can export multiple static model , so you can support a static scene.

5. The maximum vertices of a mesh should less than 32767.

