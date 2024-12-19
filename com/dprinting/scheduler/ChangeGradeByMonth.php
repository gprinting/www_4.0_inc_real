<?
include_once('/var/www/html/nimda/engine/dao/EngineDAO.php');
include_once('/var/www/html/nimda/engine/common/ConnectionPool.php');


$connectionPool = new ConnectionPool();
$conn = $connectionPool->getPooledConnection();
$CommonDAO = new EngineDAO();

$datetime = new DateTime();
$datetime->modify('-1 month');

$param = array();
$param["date"] = $datetime->format('Y-m');
$rs = $CommonDAO->selectPayPriceByMonth($conn, $param);

while ($rs && !$rs->EOF) {
    print($rs->fields["member_seqno"] . " " . $rs->fields["pay_price"] . " " . $rs->fields["now_grade"] . " " . $rs->fields["grade"] ."\n");
    $param = array();
    $param["member_seqno"] = $rs->fields["member_seqno"];
    $param["now_grade"] = $rs->fields["now_grade"];
    $param["pay_price"] = $rs->fields["pay_price"];
    $param["grade"] = $rs->fields["grade"];
    $param["calculate_period"] = date('Y-m');
    $param["state"] = "1";
    $CommonDAO->insertGradeHistory($conn, $param);
    $CommonDAO->UpdateGrade($conn, $param);

    $rs->moveNext();
}


?>