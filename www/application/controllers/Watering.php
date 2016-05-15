<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Watering extends CI_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('Watering_model');
        $this->load->helper('url');
    }

	public function index()	{
		$this->load->view('main');
	}

    public function run() {
        $this->Watering_model->setOn();
        $data = $this->Watering_model->getStatus();
        echo json_encode($data);
    }

    public function stop() {
        $this->Watering_model->setOff();
        $data = $this->Watering_model->getStatus();
        echo json_encode($data);
    }

    public function changeHumidity() {
        $this->Watering_model->setHumidity($this->input->post('humidity'));
        $data = $this->Watering_model->getStatus();
        echo json_encode($data);
    }

    public function changeArduinoIP() {
        $this->Watering_model->setArduinoIP($this->input->post('ip'));
        $data = $this->Watering_model->getStatus();
        echo json_encode($data);
    }

    public function changeArduinoPort() {
        $this->Watering_model->setArduinoPort($this->input->post('port'));
        $data = $this->Watering_model->getStatus();
        echo json_encode($data);
    }

    public function saveData() {
        $data['temperature'] = $this->input->post('temperature');
        $data['air_humidity'] = $this->input->post('air_humidity');
        $data['soil_humidity'] = $this->input->post('soil_humidity');
        $data['insolation'] = $this->input->post('insolation');
        $data['water_level'] = $this->input->post('water_level');
        $this->Watering_model->setLogs($data);
    }

    public function showData() {
        $data = $this->Watering_model->getLogs();
        echo json_encode($data);
    }

    public function getStatus() {
        $data = $this->Watering_model->getStatus();
        echo json_encode($data);
    }
}
