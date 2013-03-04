<style>
.script_label
{
  float:left;
  width:200px;
}
.script_field
{
}
</style>
<?php
require_once("dbc.php");

if (isset($_POST['create']) && isset($_POST['script_name']) && isset($_POST['script_param_count']))
{
    $scriptName = $_POST['script_name'];
    $scriptParamCount = intval($_POST['script_param_count']);

    if ($scriptParamCount == 0)
    {
        echo "Copy paste this in top of script header file:<br>";
	echo "<pre>";
	$scriptName = mysql_real_escape_string($scriptName);
	$queryScriptAdd = "INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('$scriptName',$scriptParamCount);";
	echo $queryScriptAdd."\n";
	echo "</pre>";
      
    }
    else
    {
	echo "<h3>Define parameters</h3>";
	echo "<form method=POST>";
	for ($i = 1; $i <= $scriptParamCount; $i++)
	{
	    echo "Parameters $i:";
	    echo "<div>";
	    echo "<div class='script_label'>name: </div>";
	    echo "<div class='script_field'><input type='text' name='script_param_name$i' ></div>";
	    echo "</div>";
	    echo "<div>";
	    echo "<div class='script_label'>type: </div>";
	    echo "<div class='script_field'><input type='radio' value='0' name='script_param_type$i'>String<input type='radio' value='2' name='script_param_type$i'>Float<input type='radio' value='1' name='script_param_type$i' checked>Integer";
	    echo "</div>";
	}
	echo "<input type='hidden' value='define' name='define'>";
	echo "<input type='hidden' name='script_name' value='$scriptName'>";
	echo "<input type='hidden' name='script_param_count' value='$scriptParamCount'>";
	echo "<div>
	  <div class='script_label'>&nbsp;</div>
	  <div class='script_field'><input type='submit' value='Define' ></div>
	</div>";
	echo "</form>";
    }
}
else if (isset($_POST['define']) && isset($_POST['script_name']) && isset($_POST['script_param_count']))
{
    $scriptName = $_POST['script_name'];
    $scriptParamCount = intval($_POST['script_param_count']);

    if ($scriptParamCount > 0)
    {
        echo "Copy paste this in top of script header file:<br>";

	echo "<pre>";
	$scriptName = mysql_real_escape_string($scriptName);
	$queryScriptAdd = "INSERT INTO `script`(`scriptName`,`paramCount`) VALUES ('$scriptName',$scriptParamCount);";
	echo $queryScriptAdd."\n";
      	$queryId = "SET @scriptId = LAST_INSERT_ID();";
	echo $queryId."\n";
	for ($i = 1; $i <= $scriptParamCount; $i++)
	{
	    if (isset($_POST["script_param_name$i"]) && isset($_POST["script_param_type$i"]))
	    {
		$paramName = $_POST["script_param_name$i"];
		$paramType = intval($_POST["script_param_type$i"]);
		$queryInfo = "INSERT INTO `script_info`(`scriptId`,`paramId`,`name`,`type`) VALUES (@scriptId,$i,'".mysql_real_escape_string($paramName)."',$paramType);";
		echo $queryInfo."\n";
	    }
	}
	echo "</pre>";
    }
}
else
{
?>
<h3>Create new script</h3>
<form method='POST'>
<input type='hidden' value='create' name='create'>
<div>
  <div class='script_label'>Script name: </div>
  <div class='script_field'><input type='text' name='script_name' ></div>
</div>
<div>
  <div class='script_label'>Param count: </div>
  <div class='script_field'><input type='text' name='script_param_count' ></div>
</div>
<div>
  <div class='script_label'>&nbsp;</div>
  <div class='script_field'><input type='submit' value='Create' ></div>
</div>
</form>
<?php
}
?>
