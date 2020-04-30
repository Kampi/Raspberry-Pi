function onPageLoad()
{    
    var PauseButton = document.getElementById("ButtonPause");
    var StartButton = document.getElementById("ButtonStart");

    if(Paused == true)
    {
        StartButton.disabled = false;
        PauseButton.disabled = true;
    }
    else
    {
        StartButton.disabled = true;
        PauseButton.disabled = false;
    }
}

function Start() 
{
    var PauseButton = document.getElementById("ButtonPause");
    var StartButton = document.getElementById("ButtonStart");
    
    if(StartButton.disabled == false)
    {
        StartButton.disabled = true;
        PauseButton.disabled = false;
    }

    PostData("Start")
}

function Pause() 
{
    var PauseButton = document.getElementById("ButtonPause");
    var StartButton = document.getElementById("ButtonStart");
    
    if(PauseButton.disabled == false)
    {
        StartButton.disabled = false;
        PauseButton.disabled = true;
    }

    PostData("Pause")
}

function Stop() 
{
    PostData("Shutdown")
}

function PostData(Data)
{
    $.ajax(
            {
                url: "/post",
                type : 'post',
                contentType: "application/json",
                dataType : "json",
                data : JSON.stringify(Data)
            }
    )
    .done(function(Data)
        {
        }
    )
    .fail(function(jqXHR, Status, Error)
        {
            alert("Fehler: ", Status, Error);
        }
    );
}