<?
include_once($_SERVER["INC"] . "/com/nexmotion/common/util/ConnectionPool.inc");
include_once($_SERVER["INC"] . "/common_dao/ProductCommonDAO.inc");
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/CondimentDecorator.php');

class Afterprocess extends CondimentDecorator {
	var $product;
	var $name;
	var $price;
	var $sortcode;
	var $amt;

	var $dao;
	var $connectionPool;
	var $util;
	var $conn;

	var $after_name;
	var $depth;
	var $depth1;
	var $depth2;
	var $depth1_val;
	var $depth1_vh;
	var $depth1_cnt;
	var $depth1_dvs;
	var $depth2_dvs;
	var $depth1_wid;
	var $depth1_vert;
	var $depth2_wid;
	var $depth2_vert;
	var $depth1_amt;
	var $dvs;
	var $count;
	var $stan_mpcode;
	var $size_name;

	var $size_w;
	var $size_h;

	var $jarisu;

	function __construct($product) {
		$this->product = $product;
		$this->dao = new ProductCommonDAO();
		$connectionPool = new ConnectionPool();
		$this->conn = $connectionPool->getPooledConnection();
	}

	function getDescription() {
		return $this->product->getDescription() . "후공정 : " . $this->name . "(" .$this->price . ")</br>";
	}

	function setAfterprocess($sortcode, $arr_depth, $amt = '') {
		$this->sortcode = $sortcode;
		$this->amt = $amt;
		$this->after_name = $arr_depth['after_name'];
		$this->depth = $arr_depth['aft_depth'];
		$this->depth1 = $arr_depth['aft_depth1'];
		$this->depth2 = $arr_depth['aft_depth2'];
		$this->depth1_val = $arr_depth['aft_depth1_val'];
		$this->depth1_vh = $arr_depth['aft_depth1_vh'];
		$this->depth1_cnt = $arr_depth['aft_depth1_cnt'];
		$this->depth1_dvs = $arr_depth['aft_depth1_dvs'];
		$this->depth2_dvs = $arr_depth['aft_depth2_dvs'];
		$this->depth1_wid = $arr_depth['aft_depth1_wid'];
		$this->depth1_vert = $arr_depth['aft_depth1_vert'];
		$this->depth2_wid = $arr_depth['aft_depth2_wid'];
		$this->depth2_vert = $arr_depth['aft_depth2_vert'];
		$this->depth1_amt = $arr_depth['aft_depth1_amt'];
		$this->dvs = $arr_depth['dvs'];
		$this->size_name = $arr_depth['size_name'];
		$this->count = $this->product->count;
		$this->stan_mpcode = $this->product->stan_mpcode;

		/*
		$param['after_mpcode'] = $mpcode;


		if($param['after_mpcode']) {
			$rs_price = $this->dao->selectCateAftPriceList($this->conn, $param);
			$this->price = $this->getPrice($rs_price);
		} else {
			$this->price = 0;
			$this->name = '상품정보 없음';
		}
		*/
	}

	function getAfterMpcode($param) {
		$rs = $this->dao->selectCateAftInfo($this->conn, 'SEQ' ,$param);
		return $rs->fields['mpcode'];
	}

	function getPrice($rs_price) {
		return $rs_price->fields['sell_price'];
	}

	public function cost() {
		$util = new AftPriceUtil([
			"conn" => $this->conn,
			"dao"  => $this->dao
		]);

		$param = [];
		$param["cate_sortcode"] = $this->sortcode;
		$param["after_name"]    = $this->depth;
		$param["amt"]           = $this->amt;
		$param["mpcode"]        = $this->depth1_val;
		$param["depth1"]        = $this->depth1;
		$param["depth2"]        = $this->depth2;
		$param["depth3"]        = $this->depth3;
		$param["size"]          = "";
		$ret = $util->getAfterPrice1($param);
		$ret = ceil($ret * $this->getProduct()->discount_rate);
		$ret = ceil($ret / 1.1 / 100) * 100 * 1.1;
		$result = $this->product->cost();
		$result[$this->after_name] = $ret * $this->product->count;
		$result["add_after_price"] = $result["add_after_price"] + $result[$this->after_name];
		$result["sell_price"] = $result["sell_price"] + ($ret * $this->product->count);
		$result["dvs"] = $this->dvs;

		return $result;
	}

	function getOptionList() {
		return $this->product->getOptionList();
	}

	function getAfterList() {
		$param = array();
		$param['mpcode'] = $this->depth1_val;
		if($this->startsWith($this->sortcode, "002")) {
			$rs = $this->dao->selectCateAoAfterInfo($this->conn, $param);
		} else {
			$rs = $this->dao->selectCateAfterInfo($this->conn, $param);
		}
		while ($rs && !$rs->EOF) {
			$name = $rs->fields["after_name"];
			$depth1 = $rs->fields["depth1"];
			$depth2 = $rs->fields["depth2"];
			$depth3 = $rs->fields["depth3"];
			$depth1_dvs = $this->depth1_dvs;
			$mpcode = $rs->fields["mpcode"];
			$basic_yn = $rs->fields["basic_yn"];

			$ret["after_eng_name"] = $this->after_name;
			$ret["mpcode"]     = $mpcode;
			$ret["after_name"] = $name;
			$ret["depth1"]     = $depth1;
			$ret["depth2"]     = $depth2;
			$ret["depth3"]     = $depth3;
			$ret["basic_yn"]     = $basic_yn;

			$ret["detail"] = "";
			if($depth1 != "" && $depth1 != "-") {
				$ret["detail"] .= " " . $depth1;
			}

			if($depth2 != "" && $depth2 != "-") {
				$ret["detail"] .= " " . $depth2;
			}

			if($depth3 != "" && $depth3 != "-") {
				$ret["detail"] .= " " . $depth3;
			}

			if($depth1_dvs != "" && $depth1_dvs != "-") {
				$ret["detail"] .= " " . $depth1_dvs;
			}
			$ret["detail"] = trim($ret["detail"]);
			//$ret[$i]["price"]      = $info["price"];
			//$ret[$i]["detail"]     = $info["detail"];
			//$ret[$i]["info"]       = $info["info"];
			//$ret[$i++]["seq"]        = $i + 1;
			//$ret[$i++]["basic_yn"] = 'N';

			$rs->MoveNext();
		}
		//$result = $this->product->getAfterList();
		//array_push($result,$ret);
		return $ret;
	}

	function getJarisu() {
		return $this->product->jarisu;
	}

	function getProduct() {
		return $this->product->getProduct();
	}

	function makeOrderCommonInsertParam($fb, $session) {
		return $this->product->makeOrderCommonInsertParam($fb, $session);
	}

	function makeOrderDetailInsertParam($fb, $session) {
		return $this->product->makeOrderDetailInsertParam($fb, $session);
	}

	function makeOrderDlvrInsertParam($fb) {
		return $this->product->makeOrderDlvrInsertParam($fb);
	}

	function makeOrderAfterHistoryInsertParam($fb) {
		$param = $this->getAfterList();
		$p = $this->getProduct();
		if($p->order_after_history_param == null)
			$p->order_after_history_param = array();
		array_push($p->order_after_history_param, $param);
		return $this->product->makeOrderAfterHistoryInsertParam($fb);
	}

	function makeOrderOptionHistoryInsertParam($fb) {
		return $this->product->makeOrderOptionHistoryInsertParam($fb);
	}

	function makeOrderFileInsertParam($fb) {
		return $this->product->makeOrderFileInsertParam($fb);
	}

	function startsWith($haystack, $needle) {
		// search backwards starting from haystack length characters from the end
		return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== false;
	}
}


?>
