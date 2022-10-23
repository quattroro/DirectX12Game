#pragma once
#include "Object.h"

//�ش� ���̴��� ���۵����� ������ ����
enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
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
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
};

// [�ϰ� �����] ���� �ηµ��� �� �ؾ����� ���
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path, ShaderInfo info = ShaderInfo(), const string& vs = "VS_Main", const string& ps = "PS_Main");
	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ShaderInfo _info;

	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

