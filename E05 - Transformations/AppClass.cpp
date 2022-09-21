#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	// Edit: Jonathan Karcher (9/21/2022)
	m_pMeshs[0] = new MyMesh();
	m_pMeshs[1] = new MyMesh();
	m_pMeshs[2] = new MyMesh();
	m_pMeshs[3] = new MyMesh();
	m_pMeshs[4] = new MyMesh();
	m_pMeshs[5] = new MyMesh();
	m_pMeshs[6] = new MyMesh();
	m_pMeshs[0]->GenerateCube(1.0f, C_BLACK);
	m_pMeshs[1]->GenerateCube(1.0f, C_BLACK);
	m_pMeshs[2]->GenerateCube(1.0f, C_BLACK);
	m_pMeshs[3]->GenerateCube(1.0f, C_BLACK);
	m_pMeshs[4]->GenerateCube(1.0f, C_BLACK);
	m_pMeshs[5]->GenerateCube(1.0f, C_BLACK);
	m_pMeshs[6]->GenerateCube(1.0f, C_BLACK);
	// end
		
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	// Edit: Jonathan Karcher (9/21/2022)
	static float speed = 0;
	matrix4 m4Position = glm::translate(vector3(1.0f, 1.0f, 1.0f)) * glm::translate(vector3(0.0f + speed, 0.0f, 0.0f));
	m_pMeshs[0]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(-1.0f, 0.0f, 0.0f)));
	m_pMeshs[1]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1.0f, 0.0f, 0.0f)));
	m_pMeshs[2]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 0.0f, 0.0f)));
	m_pMeshs[3]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 1.0f, 0.0f)));
	m_pMeshs[4]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, -1.0f, 0.0f)));
	m_pMeshs[5]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 0.0f, 1.0f)));
	m_pMeshs[6]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 0.0f, -1.0f)));
	speed+=.01;
	// end

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	// Edit: Jonathan Karcher (9/21/2022)
	for (int i = 0; i < meshSize; i++)
	{
		if (m_pMeshs[i] != nullptr)
		{
			delete m_pMeshs[i];
			m_pMeshs[i] = nullptr;
		}
	}
	// end

	//release GUI
	ShutdownGUI();
}