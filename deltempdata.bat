@echo off
FOR %%a in (Debug\*.ilk Debug\*.exp Debug\*.pdb Release\*.ilk Release\*.exp Release\*.pdb) do del %%a
rmdir /S /Q Debug
rmdir /S /Q Release
del Antis-GameEngine(Arbeitsname).ncb
del Antis-GameEngine(Arbeitsname).suo4
del Antis-GameEngine(Arbeitsname)\Antis-GameEngine(Arbeitsname).vcproj.anti-freak-PC.anti-freak.user
del Antis-GameEngine(Arbeitsname)\Game\Logfile.log

rmdir /S /Q Antis-GameEngine(Arbeitsname)\Debug
rmdir /S /Q Antis-GameEngine(Arbeitsname)\Release
rmdir /S /Q MapEditor\Debug
rmdir /S /Q MapEditor\Release