double Klaman(double rawdata, double prev_est) {
  double a_priori_est, a_post_est, a_priori_var, a_post_var, kalman_gain;

  a_priori_est = prev_est;
  a_priori_var = last_var + process_noise;

  kalman_gain = a_priori_var/(a_priori_var + sensor_noise);
  a_post_est = a_priori_est + kalman_gain*(rawdata - a_priori_est);
  a_post_var = (1 - kalman_gain) * a_priori_var;
  last_var = a_post_var;
  return a_post_est;
}

// need to define:
// last_var
// sensor_noise