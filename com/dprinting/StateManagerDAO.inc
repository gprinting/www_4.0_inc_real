<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-11-06
 * Time: 오후 6:54
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');
include_once(INC_PATH . "/common_lib/cypress_file.inc");

class StateManagerDAO extends NimdaCommonDAO
{
    function __construct()
    {
    }

    function selectOutputWaiting($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n SELECT * FROM ";
        $query .= "\n sheet_typset ";
        $query .= "\n where state = %s ";

        $query = sprintf($query, $param['state']);

        return $conn->Execute($query);
    }

    function selectDeliveryWating($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT A.order_detail_file_num, C.title, B.order_detail, C.amt, C.amt_unit_dvs, C.order_regi_date, C.order_common_seqno, A.state, D.invo_cpn, A.barcode_num ";
        $query .= "\n   FROM order_detail_count_file As A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON C.order_common_seqno = D.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n   WHERE A.state = '3320' AND D.dlvr_way = %s ";

        $query = sprintf($query, $param['kind']);

        return $conn->Execute($query);
    }

    function selectInHyunWaiting($conn, $param) {
        $query = "\n    SELECT A.order_detail_file_num, C.title, B.order_detail, C.amt, C.amt_unit_dvs, C.order_regi_date, C.order_common_seqno, A.state, D.invo_cpn, A.barcode_num ";
        $query .= "\n   FROM order_detail_count_file As A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON C.order_common_seqno = D.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n   WHERE A.state = '%s' AND D.dlvr_way = '06' ";

        $query = sprintf($query, $param['state']);

        return $conn->Execute($query);
    }

    function selectAfterProcessWaiting($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT A.order_detail_file_num, C.title, B.order_detail, C.amt, C.amt_unit_dvs, C.order_regi_date, C.order_common_seqno, A.state, A.barcode_num ";
        $query .= "\n   FROM order_detail_count_file As A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= "\n   WHERE A.state = %s ";

        $query = sprintf($query, $param['state']);

        return $conn->Execute($query);
    }

    function selectReleaseWaiting($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT A.order_detail_file_num, C.title, B.order_detail, C.amt, C.amt_unit_dvs, C.order_regi_date, C.order_common_seqno, A.state, D.invo_cpn, A.barcode_num ";
        $query .= "\n   FROM order_detail_count_file As A  ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_common AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON C.order_common_seqno = D.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n   WHERE A.state = %s AND D.dlvr_way != '06' ";

        $query = sprintf($query, $param['state']);

        return $conn->Execute($query);
    }

    function selectOutputImagePath($conn, $sheet_typset_seqno) {
        $query = "\n SELECT * FROM ";
        $query .= "\n sheet_typset_preview_file ";
        $query .= "\n where sheet_typset_seqno = '%s' ";

        $query = sprintf($query, $sheet_typset_seqno);
        $rs = $conn->Execute($query);

        $path = array();
        while($rs && !$rs->EOF) {
            $file_path = str_replace("/home/sitemgr/ndrive","http://erp.yesprinting.co.kr",$rs->fields['file_path']) . "/" . $rs->fields['save_file_name'];
            array_push($path, $file_path);
            $rs->MoveNext();
        }

        return $path;
    }

    function selectAfterProcesses($conn, $order_common_seqno) {
        $query = "\n    SELECT * ";
        $query .= "\n   FROM order_after_history  ";
        $query .= "\n   WHERE basic_yn = 'N' AND order_common_seqno = '%s' ";

        $query = sprintf($query, $order_common_seqno);
        $rs = $conn->Execute($query);

        $str = "";
        while($rs && !$rs->EOF) {
            $str .= "," . $rs->fields['after_name'] . "(" . $rs->fields['depth1'] . ")" . "";
            $rs->MoveNext();
        }

        return $str;
    }

}