#pragma once
#include "Object.h"

//해당 셰이더가 디퍼드인지 포워드 인지
enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	COMPUTE,
	SHADOW,
};

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

	//빛 연산을 할때는 깊이를 신경쓰면 안되기 때문에 이런 옵션들이 필요
	NO_DEPTH_TEST, // 깊이 테스트(X) + 깊이 기록(O)
	NO_DEPTH_TEST_NO_WRITE, // 깊이 테스트(X) + 깊이 기록(X)
	LESS_NO_WRITE, // 깊이 테스트(O) + 깊이 기록(X)

};

//픽셀 셰이더에 의해 만들어진 결과물이랑 렌더 타겟에 있는 결과물이랑 어떻게 섞을 것인지에 대한 속성
enum class BLEND_TYPE : uint8
{
	DEFAULT,//덮어쓰기
	ALPHA_BLEND,//알파처리
	ONE_TO_ONE_BLEND,//1대1
	END,
};

//셰이더 정보 Init을 할때 같이 받아준다.
struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};


//사용하는 셰이더의 종류가 많아지면서 CreateShader를 할때 한번에 관리하기 위해서 새롭게 추가
struct ShaderArg
{
	const string vs = "VS_Main";
	const string hs;
	const string ds;
	const string gs;
	const string ps = "PS_Main";
};


// [일감 기술서] 외주 인력들이 뭘 해야할지 기술
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void CreateGraphicsShader(const wstring& path, ShaderInfo info = ShaderInfo(), ShaderArg arg = ShaderArg());
	void CreateComputeShader(const wstring& path, const string& name, const string& version);

	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreateHullShader(const wstring& path, const string& name, const string& version);
	void CreateDomainShader(const wstring& path, const string& name, const string& version);
	void CreateGeometryShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ShaderInfo _info;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	

	// GraphicsShader
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_hsBlob;
	ComPtr<ID3DBlob>					_dsBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _graphicsPipelineDesc = {};
	
	// ComputeShader
	ComPtr<ID3DBlob>					_csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC   _computePipelineDesc = {};
};

