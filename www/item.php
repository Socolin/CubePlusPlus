<style>
a
{
    color:#8888FF;
    text-decoration:none;
}
.item
{
    width:300px;
    float:left;
}
.item_id
{
    position:absolute;
    margin-top:20px;
    color: white;
    text-shadow: -1px 0 black, 0 1px black, 1px 0 black, 0 -1px black;
    text-align:right;
}
.item_img
{
    margin-top:4px;
    float:left;
}
.item_noscript
{
    color:#FF5555;
}
.item_label
{
  float:left;
  width:200px;
}
.item_field input
{
}
.item_field_error
{
    background-color:#FF5555;
}
</style>
<?php
require_once("dbc.php");

function listBlock($mysqli)
{
    $resultBlockList = $mysqli->query("SELECT `itemId`,`name`,`scriptName`,`scriptId`
	FROM `items`
	LEFT OUTER JOIN `script` ON `scriptId`=`script`.`id`
	ORDER BY `itemId`");
	
    while ($item = $resultBlockList->fetch_row())
    {
	$id = $item[0];
	$name = $item[1];
	$script = $item[2];
	$scriptId = $item[3];
	echo "<div class='item'>";
	echo "<div class='item_id'>$id</div>";
	echo "<div class='item_img'><img src='32x32/$id.png'></div>";
	echo "<div class='item_name'><a href=?itemid=$id>$name</a></div>";
	if ($scriptId == 0)
	{
	    echo "<div class='item_noscript'>no_script</div>";
	}
	else
	{
	    echo "<div class='item_script'>$script</div>";
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
    $itemId = intval($_POST['item_id']);

    $mysqli->query("DELETE FROM `script_data` WHERE `stuffId`=$itemId AND `scriptId`=$oldScript");
    $mysqli->query("UPDATE `items` SET `scriptId`=$scriptId WHERE itemId=$itemId");

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
		$mysqli->query("REPLACE INTO `script_data`(`scriptId`,`stuffId`,`param`,`valuestr`) VALUES($scriptId,$itemId,$paramId,'".mysql_real_escape_string($_POST["param$paramId"])."')");
		break;
	      case 1:
		$mysqli->query("REPLACE INTO `script_data`(`scriptId`,`stuffId`,`param`,`valueint`) VALUES($scriptId,$itemId,$paramId,".intval($_POST["param$paramId"]).")");
		break;
	      case 2:
		$mysqli->query("REPLACE INTO `script_data`(`scriptId`,`stuffId`,`param`,`valuefloat`) VALUES($scriptId,$itemId,$paramId,".floatval($_POST["param$paramId"]).")");
		break;
	    }
	}
    }
	    
}
if (isset($_GET['itemid']))
{
    echo "<a href='?'>back</a>";

    $itemId = intval($_GET['itemid']);
    $resultBlockList = $mysqli->query("SELECT `paramCount`,`scriptId`
	FROM `items`
	LEFT OUTER JOIN `script` ON `scriptId`=`script`.`id`
	WHERE `itemId` = $itemId");
    if ($item = $resultBlockList->fetch_row())
    {
	$count_param = $item[0];
	$scriptId = $item[1];


	$scriptList = getScriptList($mysqli, $scriptId);
	echo "<form method='POST'>";
	echo "<input type='hidden' name='item_id' value='$itemId'>";
	echo "<input type='hidden' name='old_script_id' value='$scriptId'>";
	echo "<div>";
	echo "<div class='item_label'>Script</div>";
	echo "<div class='item_field'>$scriptList</div>";
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
		WHERE `scriptId` = $scriptId AND `stuffId`=$itemId AND `param` = $paramId
		ORDER BY `scriptId`");
	    if ($scriptData = $resultScriptData->fetch_row())
	    {
		$value = $scriptData[$paramType];
		$valueExist = true;
	    }
	
	    echo "<div>";
	    echo "<div class='item_label'>$infoName</div>";
	      echo "<div class='item_field'>\n";
   
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
		    $class="class='item_field_error'";
		echo "<input $class type='text' name='param$paramId' value='$value'>\n";
	      echo "</div>\n";
	    echo "</div>\n";
	}
	echo "<div>";
	echo "<div class='item_label'>&nbsp;</div>";
	echo "<div class='item_field'><input type='submit' value='Update'></div>";
	echo "</div>";
	echo "</form>";
    }
}
else
    listBlock($mysqli);

?>
