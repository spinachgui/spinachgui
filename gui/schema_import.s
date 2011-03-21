    .section .rodata
    .global spinxmlSchema
    .type   spinxmlSchema, @object
    .align  4
spinxmlSchema:
    .incbin "data/spinxml_schema.xsd"
	.int    0
    .global spinxmlSchemaSize
    .type   spinxmlSchemaSize, @object
    .align  4
spinxmlSchemaSize:
	.int    spinxmlSchemaSize - spinxmlSchema
		