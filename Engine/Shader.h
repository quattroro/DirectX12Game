#pragma once
#include "Object.h"

//스카이박스를 추가 하면서 컬링 모드를 바꿔줄 필요가 생깁
enum class RASTERIZER_TYPE
{
	CULL_NONE,//컬링 실행 안함
	CULL_FRONT,//시계방향 무시
	CULL_BACK,//기본상태 반시계방향 무시
	WIREFRAME,
};

//스카이박스를 추가 하면서 깊이 옵션을 변경해줄 필요가 생김
enum class DEPTH_STENCIL_TYPE
{
	LESS,//값이 작을때만 그려준다.
	LESS_EQUAL,//값이 같을때도 그려준다.
	GREATER,//값이 클때만 그려준다.
	GREATER_EQUAL,
};

//셰이더 정보 Init을 할때 같이 받아준다.
struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

// [일감 기술서] 외주 인력들이 뭘 해야할지 기술
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path, ShaderInfo info = ShaderInfo());
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

