<style>
a
{
    color:#8888FF;
    text-decoration:none;
}
.block
{
    width:300px;
    float:left;
}
.block_id
{
    position:absolute;
    margin-top:20px;
    color: white;
    text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black;
    text-align:right;
}
.block_img
{
    margin-top:4px;
    float:left;
}
.block_noscript
{
    color:#FF5555;
}
.block_label
{
  float:left;
  width:200px;
}
.block_field input
{
}
.block_field_error
{
    background-color:#FF5555;
}
</style>
<?php
require_once("dbc.php");

function listBlock($mysqli)
{
    $resultBlockList = $mysqli->query("SELECT `blockId`,`name`,`scriptName`,`scriptId`
	FROM `block`
	LEFT OUTER JOIN `script` ON `scriptId`=`script`.`id`
	ORDER BY `blockId`");
	
    while ($block = $resultBlockList->fetch_row())
    {
	$id = $block[0];
	$name = $block[1];
	$script = $block[2];
	$scriptId = $block[3];
	echo "<div class='block'>";
	echo "<div class='block_id'>$id</div>";
	echo "<div class='block_img'><img src='32x32/$id.png'></div>";
	echo "<div class='block_name'><a href=?blockid=$id>$name</a></div>";
	if ($scriptId == 0)
	{
	    echo "<div class='block_noscript'>no_script</div>";
	}
	else
	{
	    echo "<div class='block_script'>$script</div>";
	}
	echo "</div>";
    }
}

function getScriptList($mysqli, $idSelect)
{
    $scriptList = "<select name='script_id'>";
    $scriptList .= "<option value='0' >none</option>";
    
    $resultScriptList = $mysqli->query("SELECT `id`,`scriptName`
	FROM `script`
	ORDER BY `scriptName`");

    while ($script = $resultScriptList->fetch_row())
    {
	$id = $script[0];
	$name = $script[1];
	if ($id == $idSelect)
	{
	    $scriptList .= "<option value='$id' selected>$name</option>";
	}
	else
	{
	    $scriptList .= "<option value='$id'>$name</option>";
	}
    }
    $scriptList .= "</select>";
    return $scriptList;
}

if (isset($_POST['script_id']))
{
    $scriptId = intval($_POST['script_id']);
    $oldScript = intval($_POST['old_script_id']);
    $blockId = intval($_POST['block_id']);

    $mysqli->query("DELETE FROM `script_data` WHERE `stuffId`=$blockId AND `scriptId`=$oldScript");
    $mysqli->query("UPDATE `block` SET `scriptId`=$scriptId WHERE blockId=$blockId");

    $resultScriptInfo  = $mysqli->query("SELECT `name`,`paramId`,`type`
	FROM `script_info`
	WHERE `scriptId` = $scriptId
	ORDER BY `scriptId`");
    while ($scriptInfo = $resultScriptInfo->fetch_row())
    {
	$infoName = $scriptInfo[0];
	$paramId = $scriptInfo[1];
	$paramType = $scriptInfo[2];

	if (isset($_POST["param$paramId"]))
	{
	    switch ($paramType)
	    {
	      case 0:
		$mysqli->query("REPLACE INTO `script_data`(`scriptId`,`stuffId`,`param`,`valuestr`) VALUES($scriptId,$blockId,$paramId,'".mysql_real_escape_string($_POST["param$paramId"])."')");
		break;
	      case 1:
		$mysqli->query("REPLACE INTO `script_data`(`scriptId`,`stuffId`,`param`,`valueint`) VALUES($scriptId,$blockId,$paramId,".intval($_POST["param$paramId"]).")");
		break;
	      case 2:
		$mysqli->query("REPLACE INTO `script_data`(`scriptId`,`stuffId`,`param`,`valuefloat`) VALUES($scriptId,$blockId,$paramId,".floatval($_POST["param$paramId"]).")");
		break;
	    }
	}
    }
	    
}
if (isset($_GET['blockid']))
{
    echo "<a href='?'>back</a>";

    $blockId = intval($_GET['blockid']);
    $resultBlockList = $mysqli->query("SELECT `paramCount`,`scriptId`
	FROM `block`
	LEFT OUTER JOIN `script` ON `scriptId`=`script`.`id`
	WHERE `blockId` = $blockId");

    if ($block = $resultBlockList->fetch_row())
    {
	$count_param = $block[0];
	$scriptId = $block[1];


	$scriptList = getScriptList($mysqli, $scriptId);
	echo "<form method='POST'>";
	echo "<input type='hidden' name='block_id' value='$blockId'>";
	echo "<input type='hidden' name='old_script_id' value='$scriptId'>";
	echo "<div>";
	echo "<div class='block_label'>Script</div>";
	echo "<div class='block_field'>$scriptList</div>";
	echo "</div>";

	$resultScriptInfo  = $mysqli->query("SELECT `name`,`paramId`,`type`
	    FROM `script_info`
	    WHERE `scriptId` = $scriptId
	    ORDER BY `scriptId`");
	while ($scriptInfo = $resultScriptInfo->fetch_row())
	{
	    $infoName = $scriptInfo[0];
	    $paramId = $scriptInfo[1];
	    $paramType = $scriptInfo[2];

	    $value = "";
	    $valueExist = false;

	    $resultScriptData  = $mysqli->query("SELECT `valuestr`,`valueint`,`valuefloat`
		FROM `script_data`
		WHERE `scriptId` = $scriptId AND `stuffId`=$blockId AND `param` = $paramId
		ORDER BY `scriptId`");
	    if ($scriptData = $resultScriptData->fetch_row())
	    {
		$value = $scriptData[$paramType];
		$valueExist = true;
	    }
	
	    echo "<div>";
	    echo "<div class='block_label'>$infoName</div>";
	      echo "<div class='block_field'>\n";
   
		switch ($paramType)
		{
		  case 0:
		    echo "String:";
		    break;
		  case 1:
		    echo "Integer:";
		    break;
		  case 2:
		    echo "Float:";
		    break;
		}
		if (!$valueExist)
		    $class="class='block_field_error'";
		echo "<input $class type='text' name='param$paramId' value='$value'>\n";
	      echo "</div>\n";
	    echo "</div>\n";
	}
	echo "<div>";
	echo "<div class='block_label'>&nbsp;</div>";
	echo "<div class='block_field'><input type='submit' value='Update'></div>";
	echo "</div>";
	echo "</form>";
    }
}
else
    listBlock($mysqli);

?>
