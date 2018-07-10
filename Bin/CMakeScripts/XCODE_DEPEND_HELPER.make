# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.eternity.Debug:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a:
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a


PostBuild.game.Debug:
PostBuild.eternity.Debug: /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/glfw/lib/libglfw3.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicui18n.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicuuc.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_base.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libbase.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libplatform.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libsampler.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_external_snapshot.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/soil/lib/libSOIL.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletCollision.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletDynamics.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libLinearMath.a
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game


PostBuild.generator.Debug:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/clang/lib/libclang.dylib
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator


PostBuild.eternity.Release:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a:
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a


PostBuild.game.Release:
PostBuild.eternity.Release: /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/glfw/lib/libglfw3.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicui18n.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicuuc.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_base.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libbase.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libplatform.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libsampler.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_external_snapshot.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/soil/lib/libSOIL.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletCollision.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletDynamics.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libLinearMath.a
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game


PostBuild.generator.Release:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/clang/lib/libclang.dylib
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator


PostBuild.eternity.MinSizeRel:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a:
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a


PostBuild.game.MinSizeRel:
PostBuild.eternity.MinSizeRel: /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/glfw/lib/libglfw3.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicui18n.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicuuc.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_base.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libbase.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libplatform.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libsampler.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_external_snapshot.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/soil/lib/libSOIL.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletCollision.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletDynamics.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libLinearMath.a
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game


PostBuild.generator.MinSizeRel:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/clang/lib/libclang.dylib
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator


PostBuild.eternity.RelWithDebInfo:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a:
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a


PostBuild.game.RelWithDebInfo:
PostBuild.eternity.RelWithDebInfo: /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/glfw/lib/libglfw3.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicui18n.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicuuc.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_base.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libbase.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libplatform.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libsampler.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_external_snapshot.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/soil/lib/libSOIL.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletCollision.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletDynamics.a\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libLinearMath.a
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/game


PostBuild.generator.RelWithDebInfo:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator:\
	/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/clang/lib/libclang.dylib
	/bin/rm -f /Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/generator




# For each target create a dummy ruleso the target does not have to exist
/Users/terryjsmith/Documents/Projects/eternity-cmake/Bin/libeternity.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletCollision.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libBulletDynamics.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/bullet/lib/libLinearMath.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/clang/lib/libclang.dylib:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/glfw/lib/libglfw3.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/soil/lib/libSOIL.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicui18n.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libicuuc.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_base.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_external_snapshot.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libbase.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libplatform.a:
/Users/terryjsmith/Documents/Projects/eternity-cmake/Source/ThirdParty/v8/lib/libv8_libsampler.a:
