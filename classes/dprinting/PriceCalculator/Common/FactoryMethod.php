<?

class FactoryMethod
{
	function createPrintout($type) {}

	function create($type)
	{
		$obj = $this->createProduct($type);

		return $obj;
	}

    function createAP($type, $product)
    {
        $obj = $this->createAfterprocess($type, $product);

        return $obj;
    }

    function createDC($type, $product)
    {
        $obj = $this->createDiscount($type, $product);
        return $obj;
    }

    function createOP($type, $product)
    {
        $obj = $this->createOption($type, $product);
        return $obj;
    }
}

?>