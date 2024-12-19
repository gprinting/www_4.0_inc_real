<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-10-27
 * Time: 오후 5:40
 */

include_once(INC_PATH . '/com/nexmotion/job/nimda/common/NimdaCommonDAO.inc');

class StateDAO extends NimdaCommonDAO
{
    function __construct()
    {

    }

    public function selectBunInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT * ";
        $query .= "\n   FROM order_detail_count_file AS A   ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON D.order_common_seqno = B.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n   WHERE A.order_detail_file_num = %s OR D.invo_num = %s ";

        $query = sprintf($query, $param['order_detail_file_num'], $param['order_detail_file_num']);

        return $conn->Execute($query);
    }

    public function selectOrderDetailFileNumsFromBunInfo($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT order_detail_file_num ";
        $query .= "\n   FROM order_detail_count_file AS A ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS D ON D.order_common_seqno = B.order_common_seqno AND D.tsrs_dvs = '수신' ";
        $query .= "\n   WHERE D.bun_dlvr_order_num = %s AND D.bun_group = %s ";

        $query = sprintf($query, $param['bun_dlvr_order_num'], $param['bun_group']);
        return $conn->Execute($query);
    }

    public function selectDeliveryKind($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    select C.dlvr_way ";
        $query .= "\n   from order_detail_count_file AS A ";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_detail_seqno = B.order_detail_seqno ";
        $query .= "\n   INNER JOIN order_dlvr AS C ON B.order_common_seqno = C.order_common_seqno ";
        $query .= "\n   WHERE A.order_detail_file_num = %s ";

        $query = sprintf($query, $param['order_detail_file_num']);

        $rs = $conn->Execute($query);

        while ($rs && !$rs->EOF) {
            return $rs->fields['dlvr_way'];
        }
    }

    public function selectOrderDetailFileNumState($conn, $param) {
        $query = "\n    SELECT * FROM order_detail_count_file ";
        $query .= "\n   where order_detail_file_num = '" . $param['order_detail_file_num'] . "' ";

        $rs = $conn->Execute($query);

        while($rs && !$rs->EOF) {
            return $rs->fields['state'];
        }
    }

    public function updateCountFileState($conn, $param) {
        $query = "\n  UPDATE order_detail_count_file ";
        $query .= "\n   SET state = '%s' ";
        $query .= "\n   WHERE  order_detail_file_num = '%s' ";

        $query = sprintf($query, $param['to_state'], $param['order_detail_file_num']);

        $conn->Execute($query);

        $state_change_query = "\n INSERT INTO state_change_admin ";
        $state_change_query .= "\n (order_detail_file_num, before_state, aft_state) ";
        $state_change_query .= "\n VALUES";
        $state_change_query .= "\n ('%s', '%s', '%s') ";
        $state_change_query =
            sprintf($state_change_query, $param['order_detail_file_num'], $param['origin_state'], $param['to_state']);
        $conn->Execute($state_change_query);

        $param['order_num'] = substr($param['order_detail_file_num'], 1, strlen($param['order_detail_file_num']) - 5);
        if($this->isAllCountSameState($conn,$param)) {
            $this->updateCommonState($conn, $param);
            $this->updateDetailState($conn, $param);
        }
    }

    public function updateDetailState($conn, $param) {
        $query = " UPDATE /* 수동접수 - 작업수정 */ order_detail ";
        $query .= " SET state = '" . $param['to_state'] ."' ";
        $query .= "WHERE order_detail_dvs_num like '%" . $param['order_num'] . "%' ";

        $conn->Execute($query);
    }

    public function updateCommonState($conn, $param) {
        $query = " UPDATE /* 수동접수 - 작업수정 */ order_common ";
        $query .= " SET order_state = '" . $param['to_state'] ."' ";
        $query .= "WHERE order_num = '" . $param['order_num'] . "' ";

        $conn->Execute($query);
    }

    public function isAllCountSameState($conn, $param) {
        $query = "\n    SELECT state ";
        $query .= "\n   FROM order_detail_count_file ";
        $query .= "\n   WHERE order_detail_file_num like '%" . $param['order_num'] . "%'";

        $rs = $conn->Execute($query);

        while ($rs && !$rs->EOF) {
            if($rs->fields['state'] < $param['to_state']) {
                return false;
            }
            $rs->MoveNext();
        }

        return true;
    }

    public function selectCountFileState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT state ";
        $query .= "\n   FROM order_detail_count_file ";
        $query .= "\n   WHERE order_detail_file_num = %s ";

        $query = sprintf($query, $param['order_detail_file_num']);

        $rs = $conn->Execute($query);

        while ($rs && !$rs->EOF) {
            return $rs->fields['state'];
        }
    }

    function updateTypsetState($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n UPDATE sheet_typset AS A ";
        $query .= "\n INNER JOIN amt_order_detail_sheet AS B ON A.sheet_typset_seqno = B.sheet_typset_seqno  ";
        $query .= "\n set A.state = %s, B.state = %s ";
        $query .= "\n where A.sheet_typset_seqno = %s ";

        $query = sprintf($query, $param['to_state'], $param['to_state'], $param['sheet_typset_seqno']);

        $conn->Execute($query);
    }

    function DoesHaveAfterProcess($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n    SELECT * ";
        $query .= "\n   FROM order_after_history AS A";
        $query .= "\n   INNER JOIN order_detail AS B ON A.order_common_seqno = B.order_common_seqno ";
        $query .= "\n   INNER JOIN order_detail_count_file AS C ON B.order_detail_seqno = C.order_detail_seqno ";
        $query .= "\n   WHERE A.basic_yn = 'N' AND C.order_detail_file_num = %s ";

        $query = sprintf($query, $param['order_detail_file_num']);
        $rs = $conn->Execute($query);
        while($rs && !$rs->EOF) {
            echo $param['order_detail_file_num'] . " true";
            return true;
        }

        return false;
    }

    function selectOrderDetailCountFileSeqno($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "\n SELECT B.order_detail_file_num ";
        $query .= "\n FROM amt_order_detail_sheet AS A ";
        $query .= "\n INNER JOIN order_detail_count_file AS B ON A.order_detail_count_file_seqno = B.order_detail_count_file_seqno ";
        $query .= "\n where A.sheet_typset_seqno = %s ";

        $query = sprintf($query, $param['sheet_typset_seqno']);
        $rs = $conn->Execute($query);
        $order_detail_count_file_seqnos = array();
        while($rs && !$rs->EOF) {
            array_push($order_detail_count_file_seqnos, $rs->fields['order_detail_file_num']);
            $rs->MoveNext();
        }

        return $order_detail_count_file_seqnos;
    }

    function isIncludedTypset($conn, $param) {
        if ($this->connectionCheck($conn) === false) {
            return false;
        }

        $param = $this->parameterArrayEscape($conn, $param);

        $query = "  SELECT * \n ";
        $query .= " FROM amt_order_detail_sheet AS A  \n ";
        $query .= " INNER JOIN  order_detail_count_file AS B ON A.order_detail_count_file_seqno = B.order_detail_count_file_seqno  \n ";
        $query .= " WHERE B.order_detail_file_num = %s AND A.state = %s  \n ";

        $query = sprintf($query, $param['order_detail_file_num'], $param['origin_state']);
        $rs = $conn->Execute($query);

        while($rs && !$rs->EOF) {
            return false;
        }

        return true;
    }
}