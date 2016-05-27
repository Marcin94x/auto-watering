<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Watering_model extends CI_Model {

    public function setOn() {
        $data['isOn'] = true;
        $this->db->where('info_id', 1);
        $this->db->update('info', $data);
    }

    public function setOff() {
        $data['isOn'] = false;
        $this->db->where('info_id', 1);
        $this->db->update('info', $data);
    }

    public function setHumidity($humidity) {
        $data['humidity'] = $humidity;
        $this->db->where('info_id', 1);
        $this->db->update('info', $data);
    }

    public function setArduinoIP($ip) {
        $data['arduino_ip'] = $ip;
        $this->db->where('info_id', 1);
        $this->db->update('info', $data);
    }

    public function setArduinoPort($port) {
        $data['arduino_port'] = $port;
        $this->db->where('info_id', 1);
        $this->db->update('info', $data);
    }

    public function getLog() {
        $this->db->order_by('date', 'desc');
        $query = $this->db->get('logs');
        return $query->row();
    }

    public function getLogs() {
        $this->db->order_by('date', 'desc');
        $query = $this->db->get('logs');
        return $query->result_array();
    }

    public function setLogs($data) {
        $this->db->insert('logs', $data);
    }

    public function getStatus() {
        $this->db->where('info_id', 1);
        $query = $this->db->get('info');
        return $query->row();
    }
}
