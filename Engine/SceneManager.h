#pragma once

class Scene;

//�������� UI�� ǥ�� �Ҷ����� UI�� ���� ��� ī�޶� �ʿ���
//�׷��� �̷��� �������� ī�޶� ��� ���ؼ��� UI���� �ƴ��� �Ǻ��ϱ����� ��ġ�� �ʿ��ѵ�
//�װ��� Layer�̴�
enum
{
	MAX_LAYER = 32
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);


	//���̾� ���� �Լ���
	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;

	//���̾� ������ ���� ���� 
	//�̸��� ���� ���μ��θ� ������ ã�� �� �ֵ��� ���μ��� ����
	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

