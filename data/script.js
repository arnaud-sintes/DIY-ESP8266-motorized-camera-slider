var data = {
    run: false,
    speed: 2,
    direction: true,
};


function OnLoad()
{
    // run
    const run = _ => fetch( "/set?direction=" + directionButton.textContent + "&speed=" + data.speed, { method: "post" } );

    // "set" button:
    const setButton = document.getElementById( "setButton" );
    const updateSet = _ =>
    {
        setButton.textContent = [ "run", "stop" ][ !data.run ? 0 : 1 ];
    };
    updateSet();
    setButton.addEventListener( "click", _ =>
    {
        data.run = !data.run;
        updateSet();
        if( data.run ) {
            setButton.classList.add( "run" );
            run();
        }
        else {
            setButton.classList.remove( "run" );
            fetch( "/set?direction=forward&speed=0", { method: "post" } );
        }
    } );

    // "speed" label:
    const speedLabel = document.getElementById( "speedLabel" );
    const updateSpeed = _ =>
    {
        speedLabel.textContent = data.speed;
        if( data.run )
            run();
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
        if( data.run )
            run();
    };
    updateDirection();
    directionButton.addEventListener( "click", _ =>
    {
        data.direction = !data.direction;
        updateDirection();
    } );
}