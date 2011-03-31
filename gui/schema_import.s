    .section .rodata
    .global spinxmlSchema
    .align  4
spinxmlSchema:
    .incbin "data/spinxml_schema.xsd"
	.int    0
    .global spinxmlSchemaSize
    .align  4
spinxmlSchemaSize:
	.int    spinxmlSchemaSize - spinxmlSchema
