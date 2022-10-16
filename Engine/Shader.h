#pragma once
#include "Object.h"

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
};

//���̴� ���� Init�� �Ҷ� ���� �޾��ش�.
struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

// [�ϰ� �����] ���� �ηµ��� �� �ؾ����� ���
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

