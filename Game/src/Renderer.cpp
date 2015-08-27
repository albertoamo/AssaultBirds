#include "Renderer.h"
#include "BulletManager.h"
#include "Billboard.h"
#include "world.h"
#include "Technique.h"
#include "Node.h"


Renderer::Renderer(){

}

Renderer::~Renderer(){}

//This render is the normal one, uses each material attribute from each mesh
void Renderer::render(const World & obj){

	for (auto it = obj.models.begin(); it != obj.models.end(); ++it){
		
		Vector3 center = (*it)->origin;

			if (Game::instance->camera->clipper.SphereInFrustum(center.x, center.y, center.z, (*it)->totalRadius))
				(*it)->render(GL_TRIANGLES, Game::instance->camera, *obj.lights[0]);
	}

}

void Renderer::render(const Billboard & obj){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
	obj.material->shader->enable();

	Matrix44 viewProjection = Game::instance->camera->viewprojection_matrix;
	Matrix44 modelx = obj.Model;
	Matrix44 mvp = modelx * viewProjection;
	Matrix44 modelT = modelx.getRotationOnly();

	//Matrices
	obj.material->shader->setMatrix44("mvp", mvp);
	obj.material->shader->setMatrix44("model_matrix", modelx);
	obj.material->shader->setMatrix44("normal_matrix", modelT);

	obj.material->shader->setTexture("color_texture", obj.material->colormap->texture_id);

	//Posiciones de la luz y de la camara
	obj.material->shader->setVector3("light_pos", World::Instance()->lights[0]->position);
	obj.material->shader->setVector3("eye_pos", Game::instance->camera->eye);

	obj.mesh->render(GL_QUADS, obj.material->shader, true);
	obj.material->shader->disable();
	glDisable(GL_BLEND);
	glDepthMask(true);

}

void Renderer::render(const ShadowMapping & obj){

	glViewport( 0,0, obj.shadowmap->width, obj.shadowmap->height );
	obj.stack->camera->aspect =  obj.shadowmap->width / (float)obj.shadowmap->height;

	obj.shadowmap->enable();
	//glCullFace(GL_FRONT);

	for (auto it = obj.caster.begin(); it != obj.caster.end(); ++it){
		Vector3 center = (*it)->origin;
		if (Game::instance->camera->clipper.SphereInFrustum(center.x, center.y, center.z, (*it)->totalRadius))
			(*it)->render(GL_TRIANGLES, obj.stack);
	}

	//glCullFace(GL_BACK);
	obj.shadowmap->disable();

	glViewport( 0,0, Game::instance->window_width, Game::instance->window_height );
	//obj.stack->camera->aspect =  Game::instance->window_width / (float)Game::instance->window_height;
}


void Renderer::render(const Reflection & obj){

	obj.setResolution(obj.stack->camera);
	obj.reflection->enable();

	World::Instance()->models[0]->Model.setTranslation(obj.stack->camera->eye.x, obj.stack->camera->eye.y, obj.stack->camera->eye.z); //Skybox

	World::Instance()->models[0]->render(GL_TRIANGLES, obj.stack->camera, *World::Instance()->lights[0]);
	for (auto it = World::Instance()->models.begin() + 1; it != World::Instance()->models.end(); ++it){
		Vector3 center = (*it)->origin;

		if (Game::instance->camera->clipper.SphereInFrustum(center.x, center.y, center.z, (*it)->totalRadius))
			(*it)->render(GL_TRIANGLES, obj.stack);
	}

	World::Instance()->models[0]->Model.setTranslation(Game::instance->camera->eye.x, Game::instance->camera->eye.y, Game::instance->camera->eye.z); //Skybox

	obj.reflection->disable();
}

void Renderer::render(const BulletManager & obj){

	Mesh BulletsMesh;
	for (unsigned int i = 0; i < obj.bullets_vector.size(); i++)
	{
		if (obj.bullets_vector[i].ttl > 0.0)
		{
			BulletsMesh.vertices.push_back(obj.bullets_vector[i].lastPosition);
			BulletsMesh.vertices.push_back(obj.bullets_vector[i].position);
			BulletsMesh.colors.push_back(Vector4(0.0, 0.0, 0.0, 1.0));
			BulletsMesh.colors.push_back(Vector4(1.0, 0.0, 0.0, 1.0));
		}

		if (BulletsMesh.vertices.size()){
			glLineWidth(1.2);
			BulletsMesh.render(GL_LINES);
			glLineWidth(1);
			glColor3f(1,1,1);
		}
	}
}

void Renderer::render(const DebugParticle & obj){

	if (obj.mesh->vertices.size()){
		if (obj.shader != NULL){
			obj.shader->enable();
			Matrix44 viewProjection = Game::instance->camera->viewprojection_matrix;
			Matrix44 modelx = obj.model;
			Matrix44 mvp = modelx * viewProjection;
			Matrix44 modelT = modelx.getRotationOnly();

			//Matrices
			obj.shader->setMatrix44("mvp", mvp);
			obj.shader->setMatrix44("model_matrix", modelx);
			obj.shader->setMatrix44("normal_matrix", modelT);

			obj.shader->setVector3("eye_pos", Game::instance->camera->eye);

			obj.shader->setFloat("time", SDL_GetTicks() * 20);
			
			glPointSize(10);
			obj.mesh->render(GL_POINTS);
			obj.shader->disable();
		}
		else{//Fixed pipeline render, pretty useless
			obj.mesh->render(GL_POINTS);
		}
	}

}

void Renderer::render(const ForwardRendering & obj){

	obj.setResolution(Game::instance->camera);
	obj.screen->enable();

	render(*obj.world);
	obj.world->bulletM->render(this);
	obj.world->cloud->render(this);

	obj.screen->disable();
	obj.downResolution(Game::instance->camera);

}

void Renderer::render(const MinimapRendering & obj){

	glDisable(GL_CULL_FACE);
	//obj.setResolution(obj.stack->camera);
	glViewport(0, 0, RESX, RESY);
	obj.minimap->enable();


	for (auto it = World::Instance()->models.begin() + 1; it != World::Instance()->models.end() - 1; ++it){
		Vector3 center = (*it)->origin;

		if (obj.stack->camera->clipper.SphereInFrustum(center.x, center.y, center.z, (*it)->totalRadius))
			(*it)->render(GL_TRIANGLES, obj.stack);
	}
	glDisable(GL_DEPTH_TEST);
	obj.renderIcons();
	obj.minimap->disable();
	obj.downResolution(Game::instance->camera);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::render(const ScriptPoint & obj){

	obj.shader->enable();

	Matrix44 viewProjection = Game::instance->camera->viewprojection_matrix;
	Matrix44 modelx = obj.Model;
	Matrix44 mvp = modelx * viewProjection;
	Matrix44 modelT = modelx.getRotationOnly();

	//Matrices
	
	obj.shader->setMatrix44("mvp", mvp);
	obj.shader->setMatrix44("model_matrix", modelx);
	obj.shader->setMatrix44("normal_matrix", modelT);

	obj.shader->setVector3("colour", obj.color);

	obj.mesh->render(GL_TRIANGLES, obj.shader, true);
	obj.connector->render(GL_LINES, obj.shader, true);
	obj.shader->disable();

}
