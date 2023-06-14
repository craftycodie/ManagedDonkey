#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "game/cheats.hpp"
#include "interface/gui_custom_bitmap_widget.hpp"
#include "networking/tools/remote_command.hpp"

void process_attach(HMODULE hModule)
{
	set_donkey_module(hModule);
	DisableThreadLibraryCalls(hModule);
	SetProcessDPIAware();

#ifdef ISPROXY
	LoadLibraryW(L"D3DX9_43_orig.dll");
#endif // ISPROXY

	c_console::initialize("ManagedDonkey");

	apply_all_patches(false);
	apply_all_hooks(false);

	patch_gui_custom_bitmap_widget();
	patch_lz_cache_file_decompressor();
	patch_cheats();
	patch_remote_command();
}

void process_detach(HMODULE hModule)
{
	apply_all_hooks(true);
	apply_all_patches(true);

	c_console::dispose();

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		process_attach(hModule);
		break;
	case DLL_PROCESS_DETACH:
		process_detach(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

#ifndef ISPROXY
// needed for `DetourCreateProcessWithDllA`
extern "C" __declspec(dllexport) void null_export(void)
{
}
#else
#pragma comment(linker, "/export:D3DXCheckVersion=D3DX9_43_orig.D3DXCheckVersion")
#pragma comment(linker, "/export:D3DXGetDriverLevel=D3DX9_43_orig.D3DXGetDriverLevel")
#pragma comment(linker, "/export:D3DXCreateSprite=D3DX9_43_orig.D3DXCreateSprite")
#pragma comment(linker, "/export:D3DXCreateFontIndirectA=D3DX9_43_orig.D3DXCreateFontIndirectA")
#pragma comment(linker, "/export:D3DXCreateFontIndirectW=D3DX9_43_orig.D3DXCreateFontIndirectW")
#pragma comment(linker, "/export:D3DXCreateRenderToSurface=D3DX9_43_orig.D3DXCreateRenderToSurface")
#pragma comment(linker, "/export:D3DXCreateRenderToEnvMap=D3DX9_43_orig.D3DXCreateRenderToEnvMap")
#pragma comment(linker, "/export:D3DXCreateLine=D3DX9_43_orig.D3DXCreateLine")
#pragma comment(linker, "/export:D3DXCreateFontA=D3DX9_43_orig.D3DXCreateFontA")
#pragma comment(linker, "/export:D3DXCreateFontW=D3DX9_43_orig.D3DXCreateFontW")
#pragma comment(linker, "/export:D3DXCreateBuffer=D3DX9_43_orig.D3DXCreateBuffer")
#pragma comment(linker, "/export:D3DXDebugMute=D3DX9_43_orig.D3DXDebugMute")
#pragma comment(linker, "/export:D3DXCreateAnimationController=D3DX9_43_orig.D3DXCreateAnimationController")
#pragma comment(linker, "/export:D3DXCreateCompressedAnimationSet=D3DX9_43_orig.D3DXCreateCompressedAnimationSet")
#pragma comment(linker, "/export:D3DXCreateKeyframedAnimationSet=D3DX9_43_orig.D3DXCreateKeyframedAnimationSet")
#pragma comment(linker, "/export:D3DXFrameDestroy=D3DX9_43_orig.D3DXFrameDestroy")
#pragma comment(linker, "/export:D3DXFrameAppendChild=D3DX9_43_orig.D3DXFrameAppendChild")
#pragma comment(linker, "/export:D3DXFrameFind=D3DX9_43_orig.D3DXFrameFind")
#pragma comment(linker, "/export:D3DXFrameRegisterNamedMatrices=D3DX9_43_orig.D3DXFrameRegisterNamedMatrices")
#pragma comment(linker, "/export:D3DXFrameNumNamedMatrices=D3DX9_43_orig.D3DXFrameNumNamedMatrices")
#pragma comment(linker, "/export:D3DXLoadMeshHierarchyFromXInMemory=D3DX9_43_orig.D3DXLoadMeshHierarchyFromXInMemory")
#pragma comment(linker, "/export:D3DXLoadMeshHierarchyFromXW=D3DX9_43_orig.D3DXLoadMeshHierarchyFromXW")
#pragma comment(linker, "/export:D3DXLoadMeshHierarchyFromXA=D3DX9_43_orig.D3DXLoadMeshHierarchyFromXA")
#pragma comment(linker, "/export:D3DXFrameCalculateBoundingSphere=D3DX9_43_orig.D3DXFrameCalculateBoundingSphere")
#pragma comment(linker, "/export:D3DXSaveMeshHierarchyToFileA=D3DX9_43_orig.D3DXSaveMeshHierarchyToFileA")
#pragma comment(linker, "/export:D3DXSaveMeshHierarchyToFileW=D3DX9_43_orig.D3DXSaveMeshHierarchyToFileW")
#pragma comment(linker, "/export:D3DXFloat32To16Array=D3DX9_43_orig.D3DXFloat32To16Array")
#pragma comment(linker, "/export:D3DXFloat16To32Array=D3DX9_43_orig.D3DXFloat16To32Array")
#pragma comment(linker, "/export:D3DXVec2Normalize=D3DX9_43_orig.D3DXVec2Normalize")
#pragma comment(linker, "/export:D3DXVec2Hermite=D3DX9_43_orig.D3DXVec2Hermite")
#pragma comment(linker, "/export:D3DXVec2CatmullRom=D3DX9_43_orig.D3DXVec2CatmullRom")
#pragma comment(linker, "/export:D3DXVec2BaryCentric=D3DX9_43_orig.D3DXVec2BaryCentric")
#pragma comment(linker, "/export:D3DXVec2Transform=D3DX9_43_orig.D3DXVec2Transform")
#pragma comment(linker, "/export:D3DXVec2TransformArray=D3DX9_43_orig.D3DXVec2TransformArray")
#pragma comment(linker, "/export:D3DXVec2TransformCoord=D3DX9_43_orig.D3DXVec2TransformCoord")
#pragma comment(linker, "/export:D3DXVec2TransformCoordArray=D3DX9_43_orig.D3DXVec2TransformCoordArray")
#pragma comment(linker, "/export:D3DXVec2TransformNormal=D3DX9_43_orig.D3DXVec2TransformNormal")
#pragma comment(linker, "/export:D3DXVec2TransformNormalArray=D3DX9_43_orig.D3DXVec2TransformNormalArray")
#pragma comment(linker, "/export:D3DXVec3Normalize=D3DX9_43_orig.D3DXVec3Normalize")
#pragma comment(linker, "/export:D3DXVec3Hermite=D3DX9_43_orig.D3DXVec3Hermite")
#pragma comment(linker, "/export:D3DXVec3CatmullRom=D3DX9_43_orig.D3DXVec3CatmullRom")
#pragma comment(linker, "/export:D3DXVec3BaryCentric=D3DX9_43_orig.D3DXVec3BaryCentric")
#pragma comment(linker, "/export:D3DXVec3Transform=D3DX9_43_orig.D3DXVec3Transform")
#pragma comment(linker, "/export:D3DXVec3TransformArray=D3DX9_43_orig.D3DXVec3TransformArray")
#pragma comment(linker, "/export:D3DXVec3TransformCoord=D3DX9_43_orig.D3DXVec3TransformCoord")
#pragma comment(linker, "/export:D3DXVec3TransformCoordArray=D3DX9_43_orig.D3DXVec3TransformCoordArray")
#pragma comment(linker, "/export:D3DXVec3TransformNormal=D3DX9_43_orig.D3DXVec3TransformNormal")
#pragma comment(linker, "/export:D3DXVec3TransformNormalArray=D3DX9_43_orig.D3DXVec3TransformNormalArray")
#pragma comment(linker, "/export:D3DXVec3Project=D3DX9_43_orig.D3DXVec3Project")
#pragma comment(linker, "/export:D3DXVec3ProjectArray=D3DX9_43_orig.D3DXVec3ProjectArray")
#pragma comment(linker, "/export:D3DXVec3Unproject=D3DX9_43_orig.D3DXVec3Unproject")
#pragma comment(linker, "/export:D3DXVec3UnprojectArray=D3DX9_43_orig.D3DXVec3UnprojectArray")
#pragma comment(linker, "/export:D3DXVec4Cross=D3DX9_43_orig.D3DXVec4Cross")
#pragma comment(linker, "/export:D3DXVec4Normalize=D3DX9_43_orig.D3DXVec4Normalize")
#pragma comment(linker, "/export:D3DXVec4Hermite=D3DX9_43_orig.D3DXVec4Hermite")
#pragma comment(linker, "/export:D3DXVec4CatmullRom=D3DX9_43_orig.D3DXVec4CatmullRom")
#pragma comment(linker, "/export:D3DXVec4BaryCentric=D3DX9_43_orig.D3DXVec4BaryCentric")
#pragma comment(linker, "/export:D3DXVec4Transform=D3DX9_43_orig.D3DXVec4Transform")
#pragma comment(linker, "/export:D3DXVec4TransformArray=D3DX9_43_orig.D3DXVec4TransformArray")
#pragma comment(linker, "/export:D3DXMatrixDeterminant=D3DX9_43_orig.D3DXMatrixDeterminant")
#pragma comment(linker, "/export:D3DXMatrixDecompose=D3DX9_43_orig.D3DXMatrixDecompose")
#pragma comment(linker, "/export:D3DXMatrixMultiply=D3DX9_43_orig.D3DXMatrixMultiply")
#pragma comment(linker, "/export:D3DXMatrixTranspose=D3DX9_43_orig.D3DXMatrixTranspose")
#pragma comment(linker, "/export:D3DXMatrixMultiplyTranspose=D3DX9_43_orig.D3DXMatrixMultiplyTranspose")
#pragma comment(linker, "/export:D3DXMatrixInverse=D3DX9_43_orig.D3DXMatrixInverse")
#pragma comment(linker, "/export:D3DXMatrixScaling=D3DX9_43_orig.D3DXMatrixScaling")
#pragma comment(linker, "/export:D3DXMatrixTranslation=D3DX9_43_orig.D3DXMatrixTranslation")
#pragma comment(linker, "/export:D3DXMatrixRotationX=D3DX9_43_orig.D3DXMatrixRotationX")
#pragma comment(linker, "/export:D3DXMatrixRotationY=D3DX9_43_orig.D3DXMatrixRotationY")
#pragma comment(linker, "/export:D3DXMatrixRotationZ=D3DX9_43_orig.D3DXMatrixRotationZ")
#pragma comment(linker, "/export:D3DXMatrixRotationAxis=D3DX9_43_orig.D3DXMatrixRotationAxis")
#pragma comment(linker, "/export:D3DXMatrixRotationQuaternion=D3DX9_43_orig.D3DXMatrixRotationQuaternion")
#pragma comment(linker, "/export:D3DXMatrixRotationYawPitchRoll=D3DX9_43_orig.D3DXMatrixRotationYawPitchRoll")
#pragma comment(linker, "/export:D3DXMatrixTransformation=D3DX9_43_orig.D3DXMatrixTransformation")
#pragma comment(linker, "/export:D3DXMatrixTransformation2D=D3DX9_43_orig.D3DXMatrixTransformation2D")
#pragma comment(linker, "/export:D3DXMatrixAffineTransformation=D3DX9_43_orig.D3DXMatrixAffineTransformation")
#pragma comment(linker, "/export:D3DXMatrixAffineTransformation2D=D3DX9_43_orig.D3DXMatrixAffineTransformation2D")
#pragma comment(linker, "/export:D3DXMatrixLookAtRH=D3DX9_43_orig.D3DXMatrixLookAtRH")
#pragma comment(linker, "/export:D3DXMatrixLookAtLH=D3DX9_43_orig.D3DXMatrixLookAtLH")
#pragma comment(linker, "/export:D3DXMatrixPerspectiveRH=D3DX9_43_orig.D3DXMatrixPerspectiveRH")
#pragma comment(linker, "/export:D3DXMatrixPerspectiveLH=D3DX9_43_orig.D3DXMatrixPerspectiveLH")
#pragma comment(linker, "/export:D3DXMatrixPerspectiveFovRH=D3DX9_43_orig.D3DXMatrixPerspectiveFovRH")
#pragma comment(linker, "/export:D3DXMatrixPerspectiveFovLH=D3DX9_43_orig.D3DXMatrixPerspectiveFovLH")
#pragma comment(linker, "/export:D3DXMatrixPerspectiveOffCenterRH=D3DX9_43_orig.D3DXMatrixPerspectiveOffCenterRH")
#pragma comment(linker, "/export:D3DXMatrixPerspectiveOffCenterLH=D3DX9_43_orig.D3DXMatrixPerspectiveOffCenterLH")
#pragma comment(linker, "/export:D3DXMatrixOrthoRH=D3DX9_43_orig.D3DXMatrixOrthoRH")
#pragma comment(linker, "/export:D3DXMatrixOrthoLH=D3DX9_43_orig.D3DXMatrixOrthoLH")
#pragma comment(linker, "/export:D3DXMatrixOrthoOffCenterRH=D3DX9_43_orig.D3DXMatrixOrthoOffCenterRH")
#pragma comment(linker, "/export:D3DXMatrixOrthoOffCenterLH=D3DX9_43_orig.D3DXMatrixOrthoOffCenterLH")
#pragma comment(linker, "/export:D3DXMatrixShadow=D3DX9_43_orig.D3DXMatrixShadow")
#pragma comment(linker, "/export:D3DXMatrixReflect=D3DX9_43_orig.D3DXMatrixReflect")
#pragma comment(linker, "/export:D3DXQuaternionToAxisAngle=D3DX9_43_orig.D3DXQuaternionToAxisAngle")
#pragma comment(linker, "/export:D3DXQuaternionRotationMatrix=D3DX9_43_orig.D3DXQuaternionRotationMatrix")
#pragma comment(linker, "/export:D3DXQuaternionRotationAxis=D3DX9_43_orig.D3DXQuaternionRotationAxis")
#pragma comment(linker, "/export:D3DXQuaternionRotationYawPitchRoll=D3DX9_43_orig.D3DXQuaternionRotationYawPitchRoll")
#pragma comment(linker, "/export:D3DXQuaternionMultiply=D3DX9_43_orig.D3DXQuaternionMultiply")
#pragma comment(linker, "/export:D3DXQuaternionNormalize=D3DX9_43_orig.D3DXQuaternionNormalize")
#pragma comment(linker, "/export:D3DXQuaternionInverse=D3DX9_43_orig.D3DXQuaternionInverse")
#pragma comment(linker, "/export:D3DXQuaternionLn=D3DX9_43_orig.D3DXQuaternionLn")
#pragma comment(linker, "/export:D3DXQuaternionExp=D3DX9_43_orig.D3DXQuaternionExp")
#pragma comment(linker, "/export:D3DXQuaternionSlerp=D3DX9_43_orig.D3DXQuaternionSlerp")
#pragma comment(linker, "/export:D3DXQuaternionSquad=D3DX9_43_orig.D3DXQuaternionSquad")
#pragma comment(linker, "/export:D3DXQuaternionSquadSetup=D3DX9_43_orig.D3DXQuaternionSquadSetup")
#pragma comment(linker, "/export:D3DXQuaternionBaryCentric=D3DX9_43_orig.D3DXQuaternionBaryCentric")
#pragma comment(linker, "/export:D3DXPlaneNormalize=D3DX9_43_orig.D3DXPlaneNormalize")
#pragma comment(linker, "/export:D3DXPlaneIntersectLine=D3DX9_43_orig.D3DXPlaneIntersectLine")
#pragma comment(linker, "/export:D3DXPlaneFromPointNormal=D3DX9_43_orig.D3DXPlaneFromPointNormal")
#pragma comment(linker, "/export:D3DXPlaneFromPoints=D3DX9_43_orig.D3DXPlaneFromPoints")
#pragma comment(linker, "/export:D3DXPlaneTransform=D3DX9_43_orig.D3DXPlaneTransform")
#pragma comment(linker, "/export:D3DXPlaneTransformArray=D3DX9_43_orig.D3DXPlaneTransformArray")
#pragma comment(linker, "/export:D3DXColorAdjustSaturation=D3DX9_43_orig.D3DXColorAdjustSaturation")
#pragma comment(linker, "/export:D3DXColorAdjustContrast=D3DX9_43_orig.D3DXColorAdjustContrast")
#pragma comment(linker, "/export:D3DXFresnelTerm=D3DX9_43_orig.D3DXFresnelTerm")
#pragma comment(linker, "/export:D3DXCreateMatrixStack=D3DX9_43_orig.D3DXCreateMatrixStack")
#pragma comment(linker, "/export:D3DXSHEvalDirection=D3DX9_43_orig.D3DXSHEvalDirection")
#pragma comment(linker, "/export:D3DXSHRotateZ=D3DX9_43_orig.D3DXSHRotateZ")
#pragma comment(linker, "/export:D3DXSHScale=D3DX9_43_orig.D3DXSHScale")
#pragma comment(linker, "/export:D3DXSHAdd=D3DX9_43_orig.D3DXSHAdd")
#pragma comment(linker, "/export:D3DXSHDot=D3DX9_43_orig.D3DXSHDot")
#pragma comment(linker, "/export:D3DXSHMultiply2=D3DX9_43_orig.D3DXSHMultiply2")
#pragma comment(linker, "/export:D3DXSHMultiply3=D3DX9_43_orig.D3DXSHMultiply3")
#pragma comment(linker, "/export:D3DXSHMultiply4=D3DX9_43_orig.D3DXSHMultiply4")
#pragma comment(linker, "/export:D3DXSHMultiply5=D3DX9_43_orig.D3DXSHMultiply5")
#pragma comment(linker, "/export:D3DXSHMultiply6=D3DX9_43_orig.D3DXSHMultiply6")
#pragma comment(linker, "/export:D3DXSHEvalDirectionalLight=D3DX9_43_orig.D3DXSHEvalDirectionalLight")
#pragma comment(linker, "/export:D3DXSHEvalHemisphereLight=D3DX9_43_orig.D3DXSHEvalHemisphereLight")
#pragma comment(linker, "/export:D3DXSHRotate=D3DX9_43_orig.D3DXSHRotate")
#pragma comment(linker, "/export:D3DXSHEvalSphericalLight=D3DX9_43_orig.D3DXSHEvalSphericalLight")
#pragma comment(linker, "/export:D3DXSHEvalConeLight=D3DX9_43_orig.D3DXSHEvalConeLight")
#pragma comment(linker, "/export:D3DXSphereBoundProbe=D3DX9_43_orig.D3DXSphereBoundProbe")
#pragma comment(linker, "/export:D3DXBoxBoundProbe=D3DX9_43_orig.D3DXBoxBoundProbe")
#pragma comment(linker, "/export:D3DXIntersectTri=D3DX9_43_orig.D3DXIntersectTri")
#pragma comment(linker, "/export:D3DXIntersect=D3DX9_43_orig.D3DXIntersect")
#pragma comment(linker, "/export:D3DXIntersectSubset=D3DX9_43_orig.D3DXIntersectSubset")
#pragma comment(linker, "/export:D3DXCleanMesh=D3DX9_43_orig.D3DXCleanMesh")
#pragma comment(linker, "/export:D3DXValidMesh=D3DX9_43_orig.D3DXValidMesh")
#pragma comment(linker, "/export:D3DXComputeBoundingSphere=D3DX9_43_orig.D3DXComputeBoundingSphere")
#pragma comment(linker, "/export:D3DXComputeBoundingBox=D3DX9_43_orig.D3DXComputeBoundingBox")
#pragma comment(linker, "/export:D3DXGetFVFVertexSize=D3DX9_43_orig.D3DXGetFVFVertexSize")
#pragma comment(linker, "/export:D3DXGetDeclVertexSize=D3DX9_43_orig.D3DXGetDeclVertexSize")
#pragma comment(linker, "/export:D3DXFVFFromDeclarator=D3DX9_43_orig.D3DXFVFFromDeclarator")
#pragma comment(linker, "/export:D3DXGetDeclLength=D3DX9_43_orig.D3DXGetDeclLength")
#pragma comment(linker, "/export:D3DXDeclaratorFromFVF=D3DX9_43_orig.D3DXDeclaratorFromFVF")
#pragma comment(linker, "/export:D3DXWeldVertices=D3DX9_43_orig.D3DXWeldVertices")
#pragma comment(linker, "/export:D3DXOptimizeVertices=D3DX9_43_orig.D3DXOptimizeVertices")
#pragma comment(linker, "/export:D3DXCreateSkinInfo=D3DX9_43_orig.D3DXCreateSkinInfo")
#pragma comment(linker, "/export:D3DXCreateSkinInfoFVF=D3DX9_43_orig.D3DXCreateSkinInfoFVF")
#pragma comment(linker, "/export:D3DXCreateSkinInfoFromBlendedMesh=D3DX9_43_orig.D3DXCreateSkinInfoFromBlendedMesh")
#pragma comment(linker, "/export:D3DXCreateMesh=D3DX9_43_orig.D3DXCreateMesh")
#pragma comment(linker, "/export:D3DXSplitMesh=D3DX9_43_orig.D3DXSplitMesh")
#pragma comment(linker, "/export:D3DXOptimizeFaces=D3DX9_43_orig.D3DXOptimizeFaces")
#pragma comment(linker, "/export:D3DXCreateMeshFVF=D3DX9_43_orig.D3DXCreateMeshFVF")
#pragma comment(linker, "/export:D3DXCreateSPMesh=D3DX9_43_orig.D3DXCreateSPMesh")
#pragma comment(linker, "/export:D3DXGeneratePMesh=D3DX9_43_orig.D3DXGeneratePMesh")
#pragma comment(linker, "/export:D3DXSimplifyMesh=D3DX9_43_orig.D3DXSimplifyMesh")
#pragma comment(linker, "/export:D3DXComputeNormals=D3DX9_43_orig.D3DXComputeNormals")
#pragma comment(linker, "/export:D3DXComputeTangent=D3DX9_43_orig.D3DXComputeTangent")
#pragma comment(linker, "/export:D3DXComputeTangentFrame=D3DX9_43_orig.D3DXComputeTangentFrame")
#pragma comment(linker, "/export:D3DXComputeTangentFrameEx=D3DX9_43_orig.D3DXComputeTangentFrameEx")
#pragma comment(linker, "/export:D3DXConvertMeshSubsetToSingleStrip=D3DX9_43_orig.D3DXConvertMeshSubsetToSingleStrip")
#pragma comment(linker, "/export:D3DXConvertMeshSubsetToStrips=D3DX9_43_orig.D3DXConvertMeshSubsetToStrips")
#pragma comment(linker, "/export:D3DXValidPatchMesh=D3DX9_43_orig.D3DXValidPatchMesh")
#pragma comment(linker, "/export:D3DXCreatePatchMesh=D3DX9_43_orig.D3DXCreatePatchMesh")
#pragma comment(linker, "/export:D3DXCreateNPatchMesh=D3DX9_43_orig.D3DXCreateNPatchMesh")
#pragma comment(linker, "/export:D3DXLoadPatchMeshFromXof=D3DX9_43_orig.D3DXLoadPatchMeshFromXof")
#pragma comment(linker, "/export:D3DXSaveMeshToXA=D3DX9_43_orig.D3DXSaveMeshToXA")
#pragma comment(linker, "/export:D3DXSaveMeshToXW=D3DX9_43_orig.D3DXSaveMeshToXW")
#pragma comment(linker, "/export:D3DXLoadMeshFromXof=D3DX9_43_orig.D3DXLoadMeshFromXof")
#pragma comment(linker, "/export:D3DXCreatePMeshFromStream=D3DX9_43_orig.D3DXCreatePMeshFromStream")
#pragma comment(linker, "/export:D3DXLoadSkinMeshFromXof=D3DX9_43_orig.D3DXLoadSkinMeshFromXof")
#pragma comment(linker, "/export:D3DXLoadMeshFromXA=D3DX9_43_orig.D3DXLoadMeshFromXA")
#pragma comment(linker, "/export:D3DXLoadMeshFromXW=D3DX9_43_orig.D3DXLoadMeshFromXW")
#pragma comment(linker, "/export:D3DXLoadMeshFromXInMemory=D3DX9_43_orig.D3DXLoadMeshFromXInMemory")
#pragma comment(linker, "/export:D3DXLoadMeshFromXResource=D3DX9_43_orig.D3DXLoadMeshFromXResource")
#pragma comment(linker, "/export:D3DXGenerateOutputDecl=D3DX9_43_orig.D3DXGenerateOutputDecl")
#pragma comment(linker, "/export:D3DXRectPatchSize=D3DX9_43_orig.D3DXRectPatchSize")
#pragma comment(linker, "/export:D3DXTriPatchSize=D3DX9_43_orig.D3DXTriPatchSize")
#pragma comment(linker, "/export:D3DXTessellateTriPatch=D3DX9_43_orig.D3DXTessellateTriPatch")
#pragma comment(linker, "/export:D3DXTessellateRectPatch=D3DX9_43_orig.D3DXTessellateRectPatch")
#pragma comment(linker, "/export:D3DXTessellateNPatches=D3DX9_43_orig.D3DXTessellateNPatches")
#pragma comment(linker, "/export:D3DXCreateBox=D3DX9_43_orig.D3DXCreateBox")
#pragma comment(linker, "/export:D3DXCreateTorus=D3DX9_43_orig.D3DXCreateTorus")
#pragma comment(linker, "/export:D3DXCreateTeapot=D3DX9_43_orig.D3DXCreateTeapot")
#pragma comment(linker, "/export:D3DXCreatePolygon=D3DX9_43_orig.D3DXCreatePolygon")
#pragma comment(linker, "/export:D3DXCreateCylinder=D3DX9_43_orig.D3DXCreateCylinder")
#pragma comment(linker, "/export:D3DXCreateSphere=D3DX9_43_orig.D3DXCreateSphere")
#pragma comment(linker, "/export:D3DXCreateTextA=D3DX9_43_orig.D3DXCreateTextA")
#pragma comment(linker, "/export:D3DXCreateTextW=D3DX9_43_orig.D3DXCreateTextW")
#pragma comment(linker, "/export:D3DXGetImageInfoFromFileInMemory=D3DX9_43_orig.D3DXGetImageInfoFromFileInMemory")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromMemory=D3DX9_43_orig.D3DXLoadSurfaceFromMemory")
#pragma comment(linker, "/export:D3DXSaveSurfaceToFileA=D3DX9_43_orig.D3DXSaveSurfaceToFileA")
#pragma comment(linker, "/export:D3DXSaveSurfaceToFileW=D3DX9_43_orig.D3DXSaveSurfaceToFileW")
#pragma comment(linker, "/export:D3DXSaveSurfaceToFileInMemory=D3DX9_43_orig.D3DXSaveSurfaceToFileInMemory")
#pragma comment(linker, "/export:D3DXLoadVolumeFromMemory=D3DX9_43_orig.D3DXLoadVolumeFromMemory")
#pragma comment(linker, "/export:D3DXSaveVolumeToFileA=D3DX9_43_orig.D3DXSaveVolumeToFileA")
#pragma comment(linker, "/export:D3DXSaveVolumeToFileW=D3DX9_43_orig.D3DXSaveVolumeToFileW")
#pragma comment(linker, "/export:D3DXSaveVolumeToFileInMemory=D3DX9_43_orig.D3DXSaveVolumeToFileInMemory")
#pragma comment(linker, "/export:D3DXCheckTextureRequirements=D3DX9_43_orig.D3DXCheckTextureRequirements")
#pragma comment(linker, "/export:D3DXCheckCubeTextureRequirements=D3DX9_43_orig.D3DXCheckCubeTextureRequirements")
#pragma comment(linker, "/export:D3DXCheckVolumeTextureRequirements=D3DX9_43_orig.D3DXCheckVolumeTextureRequirements")
#pragma comment(linker, "/export:D3DXCreateTexture=D3DX9_43_orig.D3DXCreateTexture")
#pragma comment(linker, "/export:D3DXCreateCubeTexture=D3DX9_43_orig.D3DXCreateCubeTexture")
#pragma comment(linker, "/export:D3DXCreateVolumeTexture=D3DX9_43_orig.D3DXCreateVolumeTexture")
#pragma comment(linker, "/export:D3DXFillTexture=D3DX9_43_orig.D3DXFillTexture")
#pragma comment(linker, "/export:D3DXFillCubeTexture=D3DX9_43_orig.D3DXFillCubeTexture")
#pragma comment(linker, "/export:D3DXFillVolumeTexture=D3DX9_43_orig.D3DXFillVolumeTexture")
#pragma comment(linker, "/export:D3DXFillTextureTX=D3DX9_43_orig.D3DXFillTextureTX")
#pragma comment(linker, "/export:D3DXFillCubeTextureTX=D3DX9_43_orig.D3DXFillCubeTextureTX")
#pragma comment(linker, "/export:D3DXFillVolumeTextureTX=D3DX9_43_orig.D3DXFillVolumeTextureTX")
#pragma comment(linker, "/export:D3DXComputeNormalMap=D3DX9_43_orig.D3DXComputeNormalMap")
#pragma comment(linker, "/export:D3DXGetImageInfoFromFileA=D3DX9_43_orig.D3DXGetImageInfoFromFileA")
#pragma comment(linker, "/export:D3DXGetImageInfoFromFileW=D3DX9_43_orig.D3DXGetImageInfoFromFileW")
#pragma comment(linker, "/export:D3DXGetImageInfoFromResourceA=D3DX9_43_orig.D3DXGetImageInfoFromResourceA")
#pragma comment(linker, "/export:D3DXGetImageInfoFromResourceW=D3DX9_43_orig.D3DXGetImageInfoFromResourceW")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromFileInMemory=D3DX9_43_orig.D3DXLoadSurfaceFromFileInMemory")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromSurface=D3DX9_43_orig.D3DXLoadSurfaceFromSurface")
#pragma comment(linker, "/export:D3DXLoadVolumeFromFileInMemory=D3DX9_43_orig.D3DXLoadVolumeFromFileInMemory")
#pragma comment(linker, "/export:D3DXLoadVolumeFromVolume=D3DX9_43_orig.D3DXLoadVolumeFromVolume")
#pragma comment(linker, "/export:D3DXFilterTexture=D3DX9_43_orig.D3DXFilterTexture")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromFileA=D3DX9_43_orig.D3DXLoadSurfaceFromFileA")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromFileW=D3DX9_43_orig.D3DXLoadSurfaceFromFileW")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromResourceA=D3DX9_43_orig.D3DXLoadSurfaceFromResourceA")
#pragma comment(linker, "/export:D3DXLoadSurfaceFromResourceW=D3DX9_43_orig.D3DXLoadSurfaceFromResourceW")
#pragma comment(linker, "/export:D3DXLoadVolumeFromFileA=D3DX9_43_orig.D3DXLoadVolumeFromFileA")
#pragma comment(linker, "/export:D3DXLoadVolumeFromFileW=D3DX9_43_orig.D3DXLoadVolumeFromFileW")
#pragma comment(linker, "/export:D3DXLoadVolumeFromResourceA=D3DX9_43_orig.D3DXLoadVolumeFromResourceA")
#pragma comment(linker, "/export:D3DXLoadVolumeFromResourceW=D3DX9_43_orig.D3DXLoadVolumeFromResourceW")
#pragma comment(linker, "/export:D3DXCreateTextureFromFileInMemoryEx=D3DX9_43_orig.D3DXCreateTextureFromFileInMemoryEx")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromFileInMemoryEx=D3DX9_43_orig.D3DXCreateCubeTextureFromFileInMemoryEx")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromFileInMemoryEx=D3DX9_43_orig.D3DXCreateVolumeTextureFromFileInMemoryEx")
#pragma comment(linker, "/export:D3DXSaveTextureToFileA=D3DX9_43_orig.D3DXSaveTextureToFileA")
#pragma comment(linker, "/export:D3DXSaveTextureToFileW=D3DX9_43_orig.D3DXSaveTextureToFileW")
#pragma comment(linker, "/export:D3DXSaveTextureToFileInMemory=D3DX9_43_orig.D3DXSaveTextureToFileInMemory")
#pragma comment(linker, "/export:D3DXCreateTextureFromFileExA=D3DX9_43_orig.D3DXCreateTextureFromFileExA")
#pragma comment(linker, "/export:D3DXCreateTextureFromFileExW=D3DX9_43_orig.D3DXCreateTextureFromFileExW")
#pragma comment(linker, "/export:D3DXCreateTextureFromResourceExA=D3DX9_43_orig.D3DXCreateTextureFromResourceExA")
#pragma comment(linker, "/export:D3DXCreateTextureFromResourceExW=D3DX9_43_orig.D3DXCreateTextureFromResourceExW")
#pragma comment(linker, "/export:D3DXCreateTextureFromFileInMemory=D3DX9_43_orig.D3DXCreateTextureFromFileInMemory")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromFileExA=D3DX9_43_orig.D3DXCreateCubeTextureFromFileExA")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromFileExW=D3DX9_43_orig.D3DXCreateCubeTextureFromFileExW")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromResourceExA=D3DX9_43_orig.D3DXCreateCubeTextureFromResourceExA")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromResourceExW=D3DX9_43_orig.D3DXCreateCubeTextureFromResourceExW")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromFileInMemory=D3DX9_43_orig.D3DXCreateCubeTextureFromFileInMemory")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromFileExA=D3DX9_43_orig.D3DXCreateVolumeTextureFromFileExA")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromFileExW=D3DX9_43_orig.D3DXCreateVolumeTextureFromFileExW")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromResourceExA=D3DX9_43_orig.D3DXCreateVolumeTextureFromResourceExA")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromResourceExW=D3DX9_43_orig.D3DXCreateVolumeTextureFromResourceExW")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromFileInMemory=D3DX9_43_orig.D3DXCreateVolumeTextureFromFileInMemory")
#pragma comment(linker, "/export:D3DXCreateTextureFromFileA=D3DX9_43_orig.D3DXCreateTextureFromFileA")
#pragma comment(linker, "/export:D3DXCreateTextureFromFileW=D3DX9_43_orig.D3DXCreateTextureFromFileW")
#pragma comment(linker, "/export:D3DXCreateTextureFromResourceA=D3DX9_43_orig.D3DXCreateTextureFromResourceA")
#pragma comment(linker, "/export:D3DXCreateTextureFromResourceW=D3DX9_43_orig.D3DXCreateTextureFromResourceW")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromFileA=D3DX9_43_orig.D3DXCreateCubeTextureFromFileA")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromFileW=D3DX9_43_orig.D3DXCreateCubeTextureFromFileW")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromResourceA=D3DX9_43_orig.D3DXCreateCubeTextureFromResourceA")
#pragma comment(linker, "/export:D3DXCreateCubeTextureFromResourceW=D3DX9_43_orig.D3DXCreateCubeTextureFromResourceW")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromFileA=D3DX9_43_orig.D3DXCreateVolumeTextureFromFileA")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromFileW=D3DX9_43_orig.D3DXCreateVolumeTextureFromFileW")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromResourceA=D3DX9_43_orig.D3DXCreateVolumeTextureFromResourceA")
#pragma comment(linker, "/export:D3DXCreateVolumeTextureFromResourceW=D3DX9_43_orig.D3DXCreateVolumeTextureFromResourceW")
#pragma comment(linker, "/export:D3DXSHProjectCubeMap=D3DX9_43_orig.D3DXSHProjectCubeMap")
#pragma comment(linker, "/export:D3DXGetPixelShaderProfile=D3DX9_43_orig.D3DXGetPixelShaderProfile")
#pragma comment(linker, "/export:D3DXGetVertexShaderProfile=D3DX9_43_orig.D3DXGetVertexShaderProfile")
#pragma comment(linker, "/export:D3DXFindShaderComment=D3DX9_43_orig.D3DXFindShaderComment")
#pragma comment(linker, "/export:D3DXGetShaderSize=D3DX9_43_orig.D3DXGetShaderSize")
#pragma comment(linker, "/export:D3DXGetShaderVersion=D3DX9_43_orig.D3DXGetShaderVersion")
#pragma comment(linker, "/export:D3DXGetShaderInputSemantics=D3DX9_43_orig.D3DXGetShaderInputSemantics")
#pragma comment(linker, "/export:D3DXGetShaderOutputSemantics=D3DX9_43_orig.D3DXGetShaderOutputSemantics")
#pragma comment(linker, "/export:D3DXGetShaderSamplers=D3DX9_43_orig.D3DXGetShaderSamplers")
#pragma comment(linker, "/export:D3DXAssembleShaderFromFileA=D3DX9_43_orig.D3DXAssembleShaderFromFileA")
#pragma comment(linker, "/export:D3DXAssembleShaderFromFileW=D3DX9_43_orig.D3DXAssembleShaderFromFileW")
#pragma comment(linker, "/export:D3DXAssembleShaderFromResourceA=D3DX9_43_orig.D3DXAssembleShaderFromResourceA")
#pragma comment(linker, "/export:D3DXAssembleShaderFromResourceW=D3DX9_43_orig.D3DXAssembleShaderFromResourceW")
#pragma comment(linker, "/export:D3DXAssembleShader=D3DX9_43_orig.D3DXAssembleShader")
#pragma comment(linker, "/export:D3DXCompileShaderFromFileA=D3DX9_43_orig.D3DXCompileShaderFromFileA")
#pragma comment(linker, "/export:D3DXCompileShaderFromFileW=D3DX9_43_orig.D3DXCompileShaderFromFileW")
#pragma comment(linker, "/export:D3DXCompileShaderFromResourceA=D3DX9_43_orig.D3DXCompileShaderFromResourceA")
#pragma comment(linker, "/export:D3DXCompileShaderFromResourceW=D3DX9_43_orig.D3DXCompileShaderFromResourceW")
#pragma comment(linker, "/export:D3DXCompileShader=D3DX9_43_orig.D3DXCompileShader")
#pragma comment(linker, "/export:D3DXGetShaderConstantTable=D3DX9_43_orig.D3DXGetShaderConstantTable")
#pragma comment(linker, "/export:D3DXGetShaderConstantTableEx=D3DX9_43_orig.D3DXGetShaderConstantTableEx")
#pragma comment(linker, "/export:D3DXCreateTextureShader=D3DX9_43_orig.D3DXCreateTextureShader")
#pragma comment(linker, "/export:D3DXPreprocessShaderFromFileA=D3DX9_43_orig.D3DXPreprocessShaderFromFileA")
#pragma comment(linker, "/export:D3DXPreprocessShaderFromFileW=D3DX9_43_orig.D3DXPreprocessShaderFromFileW")
#pragma comment(linker, "/export:D3DXPreprocessShaderFromResourceA=D3DX9_43_orig.D3DXPreprocessShaderFromResourceA")
#pragma comment(linker, "/export:D3DXPreprocessShaderFromResourceW=D3DX9_43_orig.D3DXPreprocessShaderFromResourceW")
#pragma comment(linker, "/export:D3DXPreprocessShader=D3DX9_43_orig.D3DXPreprocessShader")
#pragma comment(linker, "/export:D3DXDisassembleShader=D3DX9_43_orig.D3DXDisassembleShader")
#pragma comment(linker, "/export:D3DXDisassembleEffect=D3DX9_43_orig.D3DXDisassembleEffect")
#pragma comment(linker, "/export:D3DXCreateEffectPool=D3DX9_43_orig.D3DXCreateEffectPool")
#pragma comment(linker, "/export:D3DXCreateEffectCompilerFromFileA=D3DX9_43_orig.D3DXCreateEffectCompilerFromFileA")
#pragma comment(linker, "/export:D3DXCreateEffectCompilerFromFileW=D3DX9_43_orig.D3DXCreateEffectCompilerFromFileW")
#pragma comment(linker, "/export:D3DXCreateEffectCompilerFromResourceA=D3DX9_43_orig.D3DXCreateEffectCompilerFromResourceA")
#pragma comment(linker, "/export:D3DXCreateEffectCompilerFromResourceW=D3DX9_43_orig.D3DXCreateEffectCompilerFromResourceW")
#pragma comment(linker, "/export:D3DXCreateEffectCompiler=D3DX9_43_orig.D3DXCreateEffectCompiler")
#pragma comment(linker, "/export:D3DXCreateEffectFromFileExA=D3DX9_43_orig.D3DXCreateEffectFromFileExA")
#pragma comment(linker, "/export:D3DXCreateEffectFromFileExW=D3DX9_43_orig.D3DXCreateEffectFromFileExW")
#pragma comment(linker, "/export:D3DXCreateEffectFromResourceExA=D3DX9_43_orig.D3DXCreateEffectFromResourceExA")
#pragma comment(linker, "/export:D3DXCreateEffectFromResourceExW=D3DX9_43_orig.D3DXCreateEffectFromResourceExW")
#pragma comment(linker, "/export:D3DXCreateEffectEx=D3DX9_43_orig.D3DXCreateEffectEx")
#pragma comment(linker, "/export:D3DXCreateEffectFromFileA=D3DX9_43_orig.D3DXCreateEffectFromFileA")
#pragma comment(linker, "/export:D3DXCreateEffectFromFileW=D3DX9_43_orig.D3DXCreateEffectFromFileW")
#pragma comment(linker, "/export:D3DXCreateEffectFromResourceA=D3DX9_43_orig.D3DXCreateEffectFromResourceA")
#pragma comment(linker, "/export:D3DXCreateEffectFromResourceW=D3DX9_43_orig.D3DXCreateEffectFromResourceW")
#pragma comment(linker, "/export:D3DXCreateEffect=D3DX9_43_orig.D3DXCreateEffect")
#pragma comment(linker, "/export:D3DXConcatenateMeshes=D3DX9_43_orig.D3DXConcatenateMeshes")
#pragma comment(linker, "/export:D3DXSavePRTBufferToFileA=D3DX9_43_orig.D3DXSavePRTBufferToFileA")
#pragma comment(linker, "/export:D3DXSavePRTBufferToFileW=D3DX9_43_orig.D3DXSavePRTBufferToFileW")
#pragma comment(linker, "/export:D3DXCreatePRTBuffer=D3DX9_43_orig.D3DXCreatePRTBuffer")
#pragma comment(linker, "/export:D3DXCreatePRTBufferTex=D3DX9_43_orig.D3DXCreatePRTBufferTex")
#pragma comment(linker, "/export:D3DXLoadPRTBufferFromFileA=D3DX9_43_orig.D3DXLoadPRTBufferFromFileA")
#pragma comment(linker, "/export:D3DXLoadPRTBufferFromFileW=D3DX9_43_orig.D3DXLoadPRTBufferFromFileW")
#pragma comment(linker, "/export:D3DXSavePRTCompBufferToFileA=D3DX9_43_orig.D3DXSavePRTCompBufferToFileA")
#pragma comment(linker, "/export:D3DXSavePRTCompBufferToFileW=D3DX9_43_orig.D3DXSavePRTCompBufferToFileW")
#pragma comment(linker, "/export:D3DXCreatePRTCompBuffer=D3DX9_43_orig.D3DXCreatePRTCompBuffer")
#pragma comment(linker, "/export:D3DXLoadPRTCompBufferFromFileA=D3DX9_43_orig.D3DXLoadPRTCompBufferFromFileA")
#pragma comment(linker, "/export:D3DXLoadPRTCompBufferFromFileW=D3DX9_43_orig.D3DXLoadPRTCompBufferFromFileW")
#pragma comment(linker, "/export:D3DXCreatePRTEngine=D3DX9_43_orig.D3DXCreatePRTEngine")
#pragma comment(linker, "/export:D3DXCreateTextureGutterHelper=D3DX9_43_orig.D3DXCreateTextureGutterHelper")
#pragma comment(linker, "/export:D3DXSHPRTCompSuperCluster=D3DX9_43_orig.D3DXSHPRTCompSuperCluster")
#pragma comment(linker, "/export:D3DXSHPRTCompSplitMeshSC=D3DX9_43_orig.D3DXSHPRTCompSplitMeshSC")
#pragma comment(linker, "/export:D3DXFileCreate=D3DX9_43_orig.D3DXFileCreate")
#pragma comment(linker, "/export:D3DXUVAtlasPartition=D3DX9_43_orig.D3DXUVAtlasPartition")
#pragma comment(linker, "/export:D3DXUVAtlasPack=D3DX9_43_orig.D3DXUVAtlasPack")
#pragma comment(linker, "/export:D3DXUVAtlasCreate=D3DX9_43_orig.D3DXUVAtlasCreate")
#pragma comment(linker, "/export:D3DXComputeIMTFromPerVertexSignal=D3DX9_43_orig.D3DXComputeIMTFromPerVertexSignal")
#pragma comment(linker, "/export:D3DXComputeIMTFromSignal=D3DX9_43_orig.D3DXComputeIMTFromSignal")
#pragma comment(linker, "/export:D3DXComputeIMTFromTexture=D3DX9_43_orig.D3DXComputeIMTFromTexture")
#pragma comment(linker, "/export:D3DXComputeIMTFromPerTexelSignal=D3DX9_43_orig.D3DXComputeIMTFromPerTexelSignal")
#endif // ISPROXY