
(cl:in-package :asdf)

(defsystem "turtlebot_ball_chaser-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "DriveToBall" :depends-on ("_package_DriveToBall"))
    (:file "_package_DriveToBall" :depends-on ("_package"))
  ))