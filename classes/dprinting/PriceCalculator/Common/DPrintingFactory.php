<?
include_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Common/FactoryMethod.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/CommonProduct.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Namecard.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Sticker.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Booklet.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Booklet_Mono.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Booklet_Small.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Envelope.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/FreeTomsonSticker.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/NCR.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Form.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Lottery.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Menu.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Multiple.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Door.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Magnetic.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Memo.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Greenbag.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Banner.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Placard.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Catalog.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Leaflet.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Poster.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Holder.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Single.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/Digital.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/AClassMetal.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Foil.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Press.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Impression.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Dotline.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Rounding.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Punching.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Cutting.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Embossing.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Laminex.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Foldline.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Coating.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Thomson.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Bonding.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Binding.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Coolcoating.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Ring.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/AddDotline.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/AfterLottery.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/HeatCutting.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Background.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/EnvelopeTape.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/HalfKnife.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Manufacture.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/Numbering.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/AfterProcesses/ThomsonImpression.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Discount.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/BasicMaterials/Option.php');
require_once($_SERVER["INC"] . '/classes/dprinting/PriceCalculator/Products/PrintfyEstimate.php'); //프린파이견적(2024.12.05 권혁서 추가)


class DPrintingFactory extends FactoryMethod
{
	/**
	 * {@inheritdoc}
	 */

	function createProduct($type)
	{
        //171115 변경
		$tmp_type_t = substr($type,0, 3);
		$tmp_type_s = substr($type,0, 6);
        if ($tmp_type_t == "003") {
            return new Namecard($type);
        } else if ($tmp_type_t == "011") {
			return new Digital($type);
		} else if ($type == "004003009" || $type == "004003010" || $type == "004004001") {
			return new FreeTomsonSticker($type);
		} else if ($type == "001001001") {
			return new Catalog($type);
		} else if ($tmp_type_t == "004") {
			if($type == "004001002"){
				return new Leaflet($type);
			}else{
				return new Sticker($type);
			}
		} else if ($type == "005002001") {
			return new Booklet_Mono($type);
		}
        //else if ($type == "005003001") {
		//	return new Digital($type);
		//}
		else if ($type == "005003001") {
			return new Booklet_Small($type);
		}
        else if ($tmp_type_t == "005") {
			return new Booklet($type);
		} else if ($tmp_type_t == "006") {
			return new Envelope($type);
		} else if ($tmp_type_s == "007001") {
			return new NCR($type);
		} else if ($tmp_type_s == "007002") {
			return new Form($type);
		} else if ($type == "008001001") {
			return new Lottery($type);
		} else if ($type == "008001002") {
			return new Menu($type);
		} else if ($type == "008001003") {
			return new Multiple($type);
		} else if ($type == "008001004") {
			return new Multiple($type);
		} else if ($type == "008001005") {
			return new Door($type);
		} else if ($type == "008002001") {
			return new Magnetic($type);
		} else if ($type == "008001006") {
			return new Memo($type);
		} else if ($tmp_type_s == "009001") {
			return new Greenbag($type);
		} else if ($type == "002002001"
				|| $type == "002002003"
				|| $type == "002002002"
				|| $type == "002001001"
				|| $type == "002005001"
				|| $type == "002004001") {
			return new Banner($type);
		} else if ($tmp_type_s == "002005") {
			return new Placard($type);
		} else if ($tmp_type_s == "002006") {
			return new AClassMetal($type);
		} else if ($type == "001001001") {
			return new Catalog($type);
		} else if ($type == "001002001") {
			return new Leaflet($type);
		} else if ($type == "001003001") {
			return new Poster($type);
		} else if ($type == "001004001") {
			return new Holder($type);
		} else if ($tmp_type_t == "012") {
			return new Single($type);
		
		//2024.12.05 프린파이 권혁서 추가(프린파이 견적서)
		}else if ($tmp_type_t == "015") {
			return new PrintfyEstimate($type);
		}else {
            return new Leaflet($type);
        }
	}

	function createAfterprocess($type, $product)
	{
		switch ($type) {
			case 'foil' :
				return new Foil($product);
			case 'foil1' :
				return new Foil($product);
			case 'foil2' :
				return new Foil($product);
			case 'foil3' :
				return new Foil($product);
			case 'press' :
				return new Press($product);
			case 'impression' :
				return new Impression($product);
			case 'dotline' :
				return new Dotline($product);
			case 'rounding' :
				return new Rounding($product);
			case 'punching' :
				return new Punching($product);
			case 'cutting' :
				return new Cutting($product);
			case 'embossing' :
				return new Embossing($product);
			case 'laminex' :
				return new Laminex($product);
			case 'foldline' :
				return new Foldline($product);
			case 'coating' :
				return new Coating($product);
			case 'thomson' :
				return new Thomson($product);
			case 'bonding' :
				return new Bonding($product);
			case 'binding' :
				return new Binding($product);
			case 'cool_coating' :
				return new Coolcoating($product);
			case 'ring' :
				return new Ring($product);
			case 'add_dotline' :
				return new AddDotline($product);
			case 'heat_cutting' :
				return new HeatCutting($product);
			case 'background' :
				return new Background($product);
			case 'envelopetape' :
				return new EnvelopeTape($product);
			case 'halfknife' :
				return new HalfKnife($product);
			case 'manufacture' :
				return new Manufacture($product);
			case 'afterlottery' :
				return new AfterLottery($product);
			case 'numbering' :
				return new Numbering($product);
			case 'thomson_impression' :
				return new ThomsonImpression($product);
			default:
				return new Afterprocess($product);
		}
	}

	function createDiscount($type, $product)
	{
		switch ($type) {
			default:
				return new Discount($product);
		}
	}

	function createOption($type, $product)
	{
		switch ($type) {
			default:
				return new Option($product);
		}
	}
}
?>
