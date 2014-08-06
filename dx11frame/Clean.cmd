@echo off

del /q /a:h *.suo
del /q *.sdf
del /q *.ncb
del /q *.user
rd /s /q bin
rd /s /q obj
rd /s /q debug
rd /s /q release
rd /s /q ipch

for /D %%G in ("*") do (
del /q /a:h "%%G\*.suo"
del /q "%%G\*.sdf"
del /q "%%G\*.ncb"
del /q "%%G\*.user"
rd /s /q "%%G\bin"
rd /s /q "%%G\obj"
rd /s /q "%%G\debug"
rd /s /q "%%G\release"
rd /s /q "%%G\ipch"
)
