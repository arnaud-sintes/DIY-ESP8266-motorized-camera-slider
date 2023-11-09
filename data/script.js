var data = {
    action: true,
    speed: 1,
    direction: true,
};


function OnLoad()
{
    // "set" button:
    const setButton = document.getElementById( "setButton" );
    const settings = document.getElementById( "settings" );
    const updateSet = _ =>
    {
        setButton.textContent = [ "run", "stop" ][ data.action ? 0 : 1 ];
    };
    updateSet();
    setButton.addEventListener( "click", _ =>
    {
        data.action = !data.action;
        updateSet();
        if( !data.action ) {
            setButton.classList.add( "run" );
            settings.classList.add( "run" );
            fetch( "/set?direction=" + directionButton.textContent + "&speed=" + data.speed, { method: "post" } );
        }
        else {
            setButton.classList.remove( "run" );
            settings.classList.remove( "run" );
            fetch( "/set?direction=forward&speed=0", { method: "post" } );
        }
    } );

    // "speed" label:
    const speedLabel = document.getElementById( "speedLabel" );
    const updateSpeed = _ =>
    {
        speedLabel.textContent = data.speed;
    };
    updateSpeed();

    // "speed" input:
    const speedInput = document.getElementById( "speedInput" );
    speedInput.addEventListener( "input", _event =>
    {
        data.speed = _event.target.value;
        updateSpeed();
    } );
    speedInput.setAttribute( "value", data.speed );

    // "direction" button:
    const directionButton = document.getElementById( "directionButton" );
    const updateDirection = _ =>
    {
        directionButton.textContent = [ "forward", "backward" ][ data.direction ? 0 : 1 ];
    };
    updateDirection();
    directionButton.addEventListener( "click", _ =>
    {
        data.direction = !data.direction;
        updateDirection();
    } );
}