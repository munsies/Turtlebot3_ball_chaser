; Auto-generated. Do not edit!


(cl:in-package turtlebot_ball_chaser-srv)


;//! \htmlinclude DriveToBall-request.msg.html

(cl:defclass <DriveToBall-request> (roslisp-msg-protocol:ros-message)
  ((linear_x
    :reader linear_x
    :initarg :linear_x
    :type cl:float
    :initform 0.0)
   (angular_z
    :reader angular_z
    :initarg :angular_z
    :type cl:float
    :initform 0.0))
)

(cl:defclass DriveToBall-request (<DriveToBall-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DriveToBall-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DriveToBall-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name turtlebot_ball_chaser-srv:<DriveToBall-request> is deprecated: use turtlebot_ball_chaser-srv:DriveToBall-request instead.")))

(cl:ensure-generic-function 'linear_x-val :lambda-list '(m))
(cl:defmethod linear_x-val ((m <DriveToBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader turtlebot_ball_chaser-srv:linear_x-val is deprecated.  Use turtlebot_ball_chaser-srv:linear_x instead.")
  (linear_x m))

(cl:ensure-generic-function 'angular_z-val :lambda-list '(m))
(cl:defmethod angular_z-val ((m <DriveToBall-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader turtlebot_ball_chaser-srv:angular_z-val is deprecated.  Use turtlebot_ball_chaser-srv:angular_z instead.")
  (angular_z m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DriveToBall-request>) ostream)
  "Serializes a message object of type '<DriveToBall-request>"
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'linear_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-double-float-bits (cl:slot-value msg 'angular_z))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DriveToBall-request>) istream)
  "Deserializes a message object of type '<DriveToBall-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'linear_x) (roslisp-utils:decode-double-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'angular_z) (roslisp-utils:decode-double-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DriveToBall-request>)))
  "Returns string type for a service object of type '<DriveToBall-request>"
  "turtlebot_ball_chaser/DriveToBallRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DriveToBall-request)))
  "Returns string type for a service object of type 'DriveToBall-request"
  "turtlebot_ball_chaser/DriveToBallRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DriveToBall-request>)))
  "Returns md5sum for a message object of type '<DriveToBall-request>"
  "c8652349d2a34a90a09475339e2a93d9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DriveToBall-request)))
  "Returns md5sum for a message object of type 'DriveToBall-request"
  "c8652349d2a34a90a09475339e2a93d9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DriveToBall-request>)))
  "Returns full string definition for message of type '<DriveToBall-request>"
  (cl:format cl:nil "float64 linear_x~%float64 angular_z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DriveToBall-request)))
  "Returns full string definition for message of type 'DriveToBall-request"
  (cl:format cl:nil "float64 linear_x~%float64 angular_z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DriveToBall-request>))
  (cl:+ 0
     8
     8
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DriveToBall-request>))
  "Converts a ROS message object to a list"
  (cl:list 'DriveToBall-request
    (cl:cons ':linear_x (linear_x msg))
    (cl:cons ':angular_z (angular_z msg))
))
;//! \htmlinclude DriveToBall-response.msg.html

(cl:defclass <DriveToBall-response> (roslisp-msg-protocol:ros-message)
  ((msg_feedback
    :reader msg_feedback
    :initarg :msg_feedback
    :type cl:string
    :initform ""))
)

(cl:defclass DriveToBall-response (<DriveToBall-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DriveToBall-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DriveToBall-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name turtlebot_ball_chaser-srv:<DriveToBall-response> is deprecated: use turtlebot_ball_chaser-srv:DriveToBall-response instead.")))

(cl:ensure-generic-function 'msg_feedback-val :lambda-list '(m))
(cl:defmethod msg_feedback-val ((m <DriveToBall-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader turtlebot_ball_chaser-srv:msg_feedback-val is deprecated.  Use turtlebot_ball_chaser-srv:msg_feedback instead.")
  (msg_feedback m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DriveToBall-response>) ostream)
  "Serializes a message object of type '<DriveToBall-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'msg_feedback))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'msg_feedback))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DriveToBall-response>) istream)
  "Deserializes a message object of type '<DriveToBall-response>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'msg_feedback) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'msg_feedback) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DriveToBall-response>)))
  "Returns string type for a service object of type '<DriveToBall-response>"
  "turtlebot_ball_chaser/DriveToBallResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DriveToBall-response)))
  "Returns string type for a service object of type 'DriveToBall-response"
  "turtlebot_ball_chaser/DriveToBallResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DriveToBall-response>)))
  "Returns md5sum for a message object of type '<DriveToBall-response>"
  "c8652349d2a34a90a09475339e2a93d9")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DriveToBall-response)))
  "Returns md5sum for a message object of type 'DriveToBall-response"
  "c8652349d2a34a90a09475339e2a93d9")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DriveToBall-response>)))
  "Returns full string definition for message of type '<DriveToBall-response>"
  (cl:format cl:nil "string msg_feedback~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DriveToBall-response)))
  "Returns full string definition for message of type 'DriveToBall-response"
  (cl:format cl:nil "string msg_feedback~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DriveToBall-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'msg_feedback))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DriveToBall-response>))
  "Converts a ROS message object to a list"
  (cl:list 'DriveToBall-response
    (cl:cons ':msg_feedback (msg_feedback msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'DriveToBall)))
  'DriveToBall-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'DriveToBall)))
  'DriveToBall-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DriveToBall)))
  "Returns string type for a service object of type '<DriveToBall>"
  "turtlebot_ball_chaser/DriveToBall")