This is a port of the code from Nvidia's DX12 raytracing tutorial, which was written against the DXR prototype from Windows RS4 (1803), to Windows RS5 (1809)

Nvidia's tutorial series:  
https://developer.nvidia.com/rtx/raytracing/dxr/DX12-Raytracing-tutorial-Part-1  
https://developer.nvidia.com/rtx/raytracing/dxr/DX12-Raytracing-tutorial-Part-2


Note on DXIL shaders:  
If dxil.dll is not present, shader compiltion still works but we get errors like: `Hash check failed for DXILibrary.pShaderBytecode:`