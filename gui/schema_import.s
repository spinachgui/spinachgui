    .section .rodata
    .global _spinxmlSchema
//    .type   spinxmlSchema, @object
    .align  4
_spinxmlSchema:
    .incbin "data/spinxml_schema.xsd"
    .int    0
    .global _spinxmlSchemaSize
//    .type   spinxmlSchemaSize, @object
    .align  4
_spinxmlSchemaSize:
    .int    _spinxmlSchemaSize - _spinxmlSchema
