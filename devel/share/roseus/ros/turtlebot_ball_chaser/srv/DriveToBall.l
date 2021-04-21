;; Auto-generated. Do not edit!


(when (boundp 'turtlebot_ball_chaser::DriveToBall)
  (if (not (find-package "TURTLEBOT_BALL_CHASER"))
    (make-package "TURTLEBOT_BALL_CHASER"))
  (shadow 'DriveToBall (find-package "TURTLEBOT_BALL_CHASER")))
(unless (find-package "TURTLEBOT_BALL_CHASER::DRIVETOBALL")
  (make-package "TURTLEBOT_BALL_CHASER::DRIVETOBALL"))
(unless (find-package "TURTLEBOT_BALL_CHASER::DRIVETOBALLREQUEST")
  (make-package "TURTLEBOT_BALL_CHASER::DRIVETOBALLREQUEST"))
(unless (find-package "TURTLEBOT_BALL_CHASER::DRIVETOBALLRESPONSE")
  (make-package "TURTLEBOT_BALL_CHASER::DRIVETOBALLRESPONSE"))

(in-package "ROS")





(defclass turtlebot_ball_chaser::DriveToBallRequest
  :super ros::object
  :slots (_linear_x _angular_z ))

(defmethod turtlebot_ball_chaser::DriveToBallRequest
  (:init
   (&key
    ((:linear_x __linear_x) 0.0)
    ((:angular_z __angular_z) 0.0)
    )
   (send-super :init)
   (setq _linear_x (float __linear_x))
   (setq _angular_z (float __angular_z))
   self)
  (:linear_x
   (&optional __linear_x)
   (if __linear_x (setq _linear_x __linear_x)) _linear_x)
  (:angular_z
   (&optional __angular_z)
   (if __angular_z (setq _angular_z __angular_z)) _angular_z)
  (:serialization-length
   ()
   (+
    ;; float64 _linear_x
    8
    ;; float64 _angular_z
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _linear_x
       (sys::poke _linear_x (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _angular_z
       (sys::poke _angular_z (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _linear_x
     (setq _linear_x (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _angular_z
     (setq _angular_z (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(defclass turtlebot_ball_chaser::DriveToBallResponse
  :super ros::object
  :slots (_msg_feedback ))

(defmethod turtlebot_ball_chaser::DriveToBallResponse
  (:init
   (&key
    ((:msg_feedback __msg_feedback) "")
    )
   (send-super :init)
   (setq _msg_feedback (string __msg_feedback))
   self)
  (:msg_feedback
   (&optional __msg_feedback)
   (if __msg_feedback (setq _msg_feedback __msg_feedback)) _msg_feedback)
  (:serialization-length
   ()
   (+
    ;; string _msg_feedback
    4 (length _msg_feedback)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _msg_feedback
       (write-long (length _msg_feedback) s) (princ _msg_feedback s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _msg_feedback
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _msg_feedback (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass turtlebot_ball_chaser::DriveToBall
  :super ros::object
  :slots ())

(setf (get turtlebot_ball_chaser::DriveToBall :md5sum-) "c8652349d2a34a90a09475339e2a93d9")
(setf (get turtlebot_ball_chaser::DriveToBall :datatype-) "turtlebot_ball_chaser/DriveToBall")
(setf (get turtlebot_ball_chaser::DriveToBall :request) turtlebot_ball_chaser::DriveToBallRequest)
(setf (get turtlebot_ball_chaser::DriveToBall :response) turtlebot_ball_chaser::DriveToBallResponse)

(defmethod turtlebot_ball_chaser::DriveToBallRequest
  (:response () (instance turtlebot_ball_chaser::DriveToBallResponse :init)))

(setf (get turtlebot_ball_chaser::DriveToBallRequest :md5sum-) "c8652349d2a34a90a09475339e2a93d9")
(setf (get turtlebot_ball_chaser::DriveToBallRequest :datatype-) "turtlebot_ball_chaser/DriveToBallRequest")
(setf (get turtlebot_ball_chaser::DriveToBallRequest :definition-)
      "float64 linear_x
float64 angular_z
---
string msg_feedback

")

(setf (get turtlebot_ball_chaser::DriveToBallResponse :md5sum-) "c8652349d2a34a90a09475339e2a93d9")
(setf (get turtlebot_ball_chaser::DriveToBallResponse :datatype-) "turtlebot_ball_chaser/DriveToBallResponse")
(setf (get turtlebot_ball_chaser::DriveToBallResponse :definition-)
      "float64 linear_x
float64 angular_z
---
string msg_feedback

")



(provide :turtlebot_ball_chaser/DriveToBall "c8652349d2a34a90a09475339e2a93d9")


