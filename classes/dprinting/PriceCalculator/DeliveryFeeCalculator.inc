<?php
/**
 * Created by PhpStorm.
 * User: Hyeonsik Cho
 * Date: 2017-09-14
 * Time: 오전 11:57
 */

define(INC_PATH, $_SERVER["INC"]);
include_once(INC_PATH . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once(INC_PATH . "/com/nexmotion/common/util/front/FrontCommonUtil.inc");
include_once(INC_PATH . '/com/dprinting/DeliveryAppDAO.inc');
include_once(INC_PATH . "/classes/dprinting/PriceCalculator/Common/DPrintingFactory.php");

class DeliveryFeeCalculator {
    var $connectionPool;
    var $conn;
    var $dao;

    public function __construct() {
        $this->connectionPool = new ConnectionPool();
        $this->conn = $this->connectionPool->getPooledConnection();
        $this->dao = new DeliveryAppDAO();
    }

    public function getDeliveryFeeInfo($seqnos, $dlvr_way, $zipcode, $member_seqno = null) {
        $seqnos = explode('|', $seqnos);
        $seqnos = $this->dao->arr2paramStr($this->conn, $seqnos);

        $param = array();
        $param["order_common_seqno"] = $seqnos;
        $param['zipcode'] = $zipcode;
        $param['member_seqno'] = $member_seqno;
        $sheet_list = $this->dao->selectCartOrderList($this->conn, $param);

        $i = 0;
        $dlvr_param = array();
        while($sheet_list && !$sheet_list->EOF) {
            $dlvr_param[$i]['order_detail'] = $sheet_list->fields['order_detail'];
            $dlvr_param[$i]['order_common_seqno'] = $sheet_list->fields['order_common_seqno'];
            $dlvr_param[$i]['amt'] = $sheet_list->fields['amt'];
            $dlvr_param[$i]['count'] = $sheet_list->fields['count'];
            $dlvr_param[$i]['order_detail'] = $sheet_list->fields['order_detail'];
            $dlvr_param[$i]['cate_sortcode'] = $sheet_list->fields['cate_sortcode'];
            $dlvr_param[$i]['expec_weight'] = $sheet_list->fields['expec_weight'];

            $i++;
            $sheet_list->moveNext();
        }


        /***********************************************************************************
         **** 가져온 데이터들을 통해 택배 운임요금 계산
         ***********************************************************************************/

        $factory = new DPrintingFactory();
        $dlvr_cost_nc = 0;
        $dlvr_cost_bl = 0;
        $weight_leaflet = 0;
        $weight_namecard = 0;
        $seq_leaflet = "";
        $seq_namecard = "";
        $boxCount = 0;
        $island_cost = 0;

        if($dlvr_way == "01") {
            $rs = $this->dao->selectIslandParcelCost($this->conn, $param);
            while ($rs && !$rs->EOF) {
                $island_cost = $rs->fields["price"];
                $rs->MoveNext();
            }
        }

        for($i=0; $i < count($dlvr_param) ; $i++) {
            $cate_sortcode = $dlvr_param[$i]['cate_sortcode'];
            $product = $factory->create($cate_sortcode);

            $sort = $product->getSort();


            // 명함은 주문건의 모든 상품을 합쳐서 배송비를 받아야함
            if ($sort == "namecard") {
                $weight_namecard += $dlvr_param[$i]['expec_weight'];
                $seq_namecard .= $dlvr_param[$i]['order_common_seqno'] . "|";
            }
            // 전단은 건당으로 배송비를 받아야함
            else if ($sort == "leaflet") {
                //$weight_leaflet += $dlvr_param[$i]['expec_weight'];
                $param['expec_weight'] = $dlvr_param[$i]['expec_weight'];
                $dlvr_cost_bl += $product->getDlvrCost($param, $dlvr_way);
                $seq_leaflet .= $dlvr_param[$i]['order_common_seqno'] . "|";
                $blBoxCount = $this->getLeafletBoxcount($dlvr_param[$i]['expec_weight']);
                $boxCount += $blBoxCount;
                $dlvr_cost_bl += $blBoxCount * $island_cost;
                $weight_leaflet += $dlvr_param[$i]['expec_weight'];
            } else { // 모든 상품들이 전단 / 명함으로 구분지어지면 삭제해야한다.
                $weight_leaflet += $dlvr_param[$i]['expec_weight'];
                $seq_leaflet .= $dlvr_param[$i]['order_common_seqno'] . "|";
            }
        }

        if($weight_namecard != 0) {
            $ncBoxCount = (int)($weight_namecard / 12) + 1;
            $boxCount += $ncBoxCount;
            $dlvr_cost_nc += $ncBoxCount * $island_cost;
        }

        if($seq_leaflet != "") {
            $seq_leaflet = substr($seq_leaflet , 0, -1);
        }

        if($seq_namecard != "") {
            $seq_namecard = substr($seq_namecard , 0, -1);
        }

        if($weight_namecard != 0) {
            $product = $factory->create("001001001");
            $param = array();
            $param['zipcode'] = $zipcode;
            $param['expec_weight'] = $weight_namecard;
            $dlvr_cost_nc += $product->getDlvrCost($param, $dlvr_way);
        }

        if($weight_leaflet != 0) {
            $product = $factory->create("002001001");
            $param = array();
            $param['zipcode'] = $zipcode;
            $param['expec_weight'] = $weight_leaflet;
            //$dlvr_cost_bl += $product->getDlvrCost($param, $dlvr_way);
        }

        $result = array();
        $result['dlvr_cost_nc'] = $dlvr_cost_nc;
        $result['dlvr_cost_bl'] = $dlvr_cost_bl;
        $result['seq_leaflet'] = $seq_leaflet;
        $result['seq_namecard'] = $seq_namecard;
        $result['island_cost'] = $island_cost;
        $result['ncBoxCount'] = $ncBoxCount;
        $result['blBoxCount'] = $blBoxCount;
        $result['weight_namecard'] = $weight_namecard;
        $result['weight_leaflet'] = $weight_leaflet;

        return $result;
    }

    private function getLeafletBoxcount($expec_weight) {
        $count = 1;

        if($expec_weight > 32) {
            $count = (int)($expec_weight / 25) + 1;
        }

        return $count;
    }
}