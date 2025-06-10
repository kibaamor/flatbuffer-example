# FlatBuffer example

```powershell
# Install FlatBuffer
git clone https://github.com/google/flatbuffers.git
cd flatbuffers
cmake -B build .
cmake --build build --config Release -j
sudo cmake --install build
# add `C:\Program Files (x86)\FlatBuffers\bin` into PATH

# Compile this example
cmake -B build .
cmake --build build --config Release -j
.\build\Release\flatbuffer-example.exe
```
