rm -rf ./dist
mkdir dist
cp pongrounds ./dist
cp pongrounds.exe ./dist
cp -r ./assets ./dist
cp -r ./lib ./dist
mv ./dist/lib/win32/raylib.dll ./dist
zip -r SuperPong ./dist/*
rm -rf dist
