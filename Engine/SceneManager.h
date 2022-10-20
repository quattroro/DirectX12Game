#pragma once

class Scene;

//엔진에서 UI를 표시 할때에는 UI만 따로 찍는 카메라가 필요함
//그런데 이렇게 여러개의 카메라를 쏘기 위해서는 UI인지 아닌지 판별하기위한 장치가 필요한데
//그것이 Layer이다
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


	//레이어 관리 함수들
	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;

	//레이어 관리를 위해 존재 
	//이름과 숫자 서로서로를 가지고 찾을 수 있도록 서로서로 매핑
	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

