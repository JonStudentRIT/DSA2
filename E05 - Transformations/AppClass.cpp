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
	for (int i = 0; i < meshSize; i++)
	{
		m_pMeshs[i] = new MyMesh();
		m_pMeshs[i]->GenerateCube(1.0f, C_BLACK);
	}
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
	m_pMeshs[0]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 0.0f, 0.0f)));
	m_pMeshs[1]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, 0.0f, 0.0f)));

	m_pMeshs[2]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, -1.0f, 0.0f)));
	m_pMeshs[3]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, -1.0f, 0.0f)));

	m_pMeshs[4]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, -2.0f, 0.0f)));
	m_pMeshs[5]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, -2.0f, 0.0f)));
	m_pMeshs[6]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, -2.0f, 0.0f)));
	m_pMeshs[7]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, -2.0f, 0.0f)));
	m_pMeshs[8]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, -2.0f, 0.0f)));
	m_pMeshs[9]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, -2.0f, 0.0f)));
	m_pMeshs[10]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, -2.0f, 0.0f)));

	m_pMeshs[11]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1.0f, -3.0f, 0.0f)));
	m_pMeshs[12]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, -3.0f, 0.0f)));
	m_pMeshs[13]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, -3.0f, 0.0f)));
	m_pMeshs[14]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, -3.0f, 0.0f)));
	m_pMeshs[15]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, -3.0f, 0.0f)));
	m_pMeshs[16]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, -3.0f, 0.0f)));
	m_pMeshs[17]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(9.0f, -3.0f, 0.0f)));

	m_pMeshs[18]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, -4.0f, 0.0f)));
	m_pMeshs[19]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(1.0f, -4.0f, 0.0f)));
	m_pMeshs[20]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, -4.0f, 0.0f)));
	m_pMeshs[21]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, -4.0f, 0.0f)));
	m_pMeshs[22]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, -4.0f, 0.0f)));
	m_pMeshs[23]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, -4.0f, 0.0f)));
	m_pMeshs[24]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, -4.0f, 0.0f)));
	m_pMeshs[25]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, -4.0f, 0.0f)));
	m_pMeshs[26]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, -4.0f, 0.0f)));
	m_pMeshs[27]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(9.0f, -4.0f, 0.0f)));
	m_pMeshs[28]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, -4.0f, 0.0f)));

	m_pMeshs[29]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, -5.0f, 0.0f)));
	m_pMeshs[30]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, -5.0f, 0.0f)));
	m_pMeshs[31]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, -5.0f, 0.0f)));
	m_pMeshs[32]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, -5.0f, 0.0f)));
	m_pMeshs[33]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(5.0f, -5.0f, 0.0f)));
	m_pMeshs[34]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, -5.0f, 0.0f)));
	m_pMeshs[35]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, -5.0f, 0.0f)));
	m_pMeshs[36]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, -5.0f, 0.0f)));
	m_pMeshs[37]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, -5.0f, 0.0f)));

	m_pMeshs[38]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, -6.0f, 0.0f)));
	m_pMeshs[39]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, -6.0f, 0.0f)));
	m_pMeshs[40]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, -6.0f, 0.0f)));
	m_pMeshs[41]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, -6.0f, 0.0f)));

	m_pMeshs[42]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, -7.0f, 0.0f)));
	m_pMeshs[43]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(4.0f, -7.0f, 0.0f)));
	m_pMeshs[44]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(6.0f, -7.0f, 0.0f)));
	m_pMeshs[45]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7.0f, -7.0f, 0.0f)));
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