
var moveFlag = 0;
var moveDirection = new Vector3();
var turnFlag = 0;
var speed = 3.0;

var MOVE_FORWARD = 10;
var MOVE_BACKWARD = 20;
var TURN_LEFT = 30;
var TURN_RIGHT = 40;

function Init() {
    Command.RegisterCommandType("MOVE_FORWARD", MOVE_FORWARD);
    Command.RegisterCommandType("MOVE_BACKWARD", MOVE_BACKWARD);
    Command.RegisterCommandType("TURN_LEFT", TURN_LEFT);
    Command.RegisterCommandType("TURN_RIGHT", TURN_RIGHT);
    
    var keyboard = InputSystem.GetInputDevice(INPUTDEVICE_KEYBOARD);
    InputSystem.RegisterInputMapping(keyboard, KEY_UP, "MOVE_FORWARD");
    InputSystem.RegisterInputMapping(keyboard, KEY_DOWN, "MOVE_BACKWARD");
    InputSystem.RegisterInputMapping(keyboard, KEY_LEFT, "TURN_LEFT");
    InputSystem.RegisterInputMapping(keyboard, KEY_RIGHT, "TURN_RIGHT");
    
    ScriptingSystem.RegisterEventHandler("Command", "OnCommand");
}

function OnCommand(command) {
    var msgType = command.GetMessageType();
    if (msgType == MOVE_FORWARD || msgType == MOVE_BACKWARD) {
        if(command.GetType() == 0) {
            moveFlag = 0;
        }
        else {
            moveFlag = (msgType == MOVE_FORWARD) ? 1 : -1;
        }
    }
    
    if (msgType == TURN_LEFT || msgType == TURN_RIGHT) {
        if(command.GetType() == 0) {
            turnFlag = 0;
        }
        else {
            turnFlag = (msgType == TURN_LEFT) ? 1 : -1;
        }
    }
}

function Update(delta){
    if(moveFlag != 0 || turnFlag != 0) {
        var camera = GameObject.FindComponent("CameraComponent");
        var transform = camera.GetTransform();
        var look = transform.GetLook();
        var up = transform.GetUp();
        
        if(moveFlag != 0) {
            moveDirection.x = look.x * delta * moveFlag * speed;
            moveDirection.y = look.y * delta * moveFlag * speed;
            moveDirection.z = look.z * delta * moveFlag * speed;
        
            transform.Move(moveDirection);
        }
        
        if(turnFlag != 0) {
            transform.Rotate(up, 45 * turnFlag * delta);
        }
    }
}

