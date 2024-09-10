#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 3;
	float theta_deg_step = 3;
	float threshold_1 = 0.45;
	float threshold_2 = 0.55;

	float R = 240;
	float r = R * 0.25;
	bool flag = true;

	for (float r = R * 0.125; r <= R * 0.25; r += R * 0.125) {

		ofColor face_color = flag ? ofColor(255) : ofColor(0);
		ofColor line_color = flag ? ofColor(0) : ofColor(255);
		flag = !flag;

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				auto noise_value = ofNoise(glm::vec4(this->make_point(R, 10, theta_deg, phi_deg) * 0.02, r * 0.005 + ofGetFrameNum() * 0.025));
				if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

				auto noise_1 = ofNoise(glm::vec4(this->make_point(R, 10, theta_deg - theta_deg_step, phi_deg) * 0.02, r * 0.005 + ofGetFrameNum() * 0.025));
				auto noise_2 = ofNoise(glm::vec4(this->make_point(R, 10, theta_deg, phi_deg + phi_deg_step) * 0.02, r * 0.005 + ofGetFrameNum() * 0.025));
				auto noise_3 = ofNoise(glm::vec4(this->make_point(R, 10, theta_deg, phi_deg - phi_deg_step) * 0.02, r * 0.005 + ofGetFrameNum() * 0.025));
				auto noise_4 = ofNoise(glm::vec4(this->make_point(R, 10, theta_deg + theta_deg_step, phi_deg) * 0.02, r * 0.005 + ofGetFrameNum() * 0.025));

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

				this->face.addVertices(vertices);

				this->face.addColor(face_color); 
				this->face.addColor(face_color); 
				this->face.addColor(face_color); 
				this->face.addColor(face_color);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				if (noise_1 <= threshold_1 || noise_1 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[2]);

					this->line.addColor(line_color);
					this->line.addColor(line_color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_2 <= threshold_1 || noise_2 >= threshold_2) {

					this->line.addVertex(vertices[2]);
					this->line.addVertex(vertices[3]);

					this->line.addColor(line_color);
					this->line.addColor(line_color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_3 <= threshold_1 || noise_3 >= threshold_2) {

					this->line.addVertex(vertices[0]);
					this->line.addVertex(vertices[1]);

					this->line.addColor(line_color);
					this->line.addColor(line_color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);
				}

				if (noise_4 <= threshold_1 || noise_4 >= threshold_2) {

					this->line.addVertex(vertices[1]);
					this->line.addVertex(vertices[3]);

					this->line.addColor(line_color);
					this->line.addColor(line_color);

					this->line.addIndex(this->line.getNumVertices() - 1);
					this->line.addIndex(this->line.getNumVertices() - 2);

				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	this->line.draw();
	this->face.draw();

	this->cam.end();

	/*
	int start = 250;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// ���w�f�b�T������ �`���Ċy���ސ��w�����@P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}