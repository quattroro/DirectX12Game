#pragma once
#include "Object.h"

//�ش� ���̴��� ���۵����� ������ ����
enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	COMPUTE,
	SHADOW,
};

//��ī�̹ڽ��� �߰� �ϸ鼭 �ø� ��带 �ٲ��� �ʿ䰡 ����
enum class RASTERIZER_TYPE
{
	CULL_NONE,//�ø� ���� ����
	CULL_FRONT,//�ð���� ����
	CULL_BACK,//�⺻���� �ݽð���� ����
	WIREFRAME,
};

//��ī�̹ڽ��� �߰� �ϸ鼭 ���� �ɼ��� �������� �ʿ䰡 ����
enum class DEPTH_STENCIL_TYPE
{
	LESS,//���� �������� �׷��ش�.
	LESS_EQUAL,//���� �������� �׷��ش�.
	GREATER,//���� Ŭ���� �׷��ش�.
	GREATER_EQUAL,

	//�� ������ �Ҷ��� ���̸� �Ű澲�� �ȵǱ� ������ �̷� �ɼǵ��� �ʿ�
	NO_DEPTH_TEST, // ���� �׽�Ʈ(X) + ���� ���(O)
	NO_DEPTH_TEST_NO_WRITE, // ���� �׽�Ʈ(X) + ���� ���(X)
	LESS_NO_WRITE, // ���� �׽�Ʈ(O) + ���� ���(X)

};

//�ȼ� ���̴��� ���� ������� ������̶� ���� Ÿ�ٿ� �ִ� ������̶� ��� ���� �������� ���� �Ӽ�
enum class BLEND_TYPE : uint8
{
	DEFAULT,//�����
	ALPHA_BLEND,//����ó��
	ONE_TO_ONE_BLEND,//1��1
	END,
};

//���̴� ���� Init�� �Ҷ� ���� �޾��ش�.
struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};


//����ϴ� ���̴��� ������ �������鼭 CreateShader�� �Ҷ� �ѹ��� �����ϱ� ���ؼ� ���Ӱ� �߰�
struct ShaderArg
{
	const string vs = "VS_Main";
	const string hs;
	const string ds;
	const string gs;
	const string ps = "PS_Main";
};


// [�ϰ� �����] ���� �ηµ��� �� �ؾ����� ���
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

